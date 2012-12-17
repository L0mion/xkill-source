#include <iostream>

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>
#include <xkill-utilities/MeshModel.h>
#include <xkill-utilities/TexDescHeader.h>
#include <xkill-utilities/TexDescTex.h>
#include <xkill-utilities/TexDesc.h>

#include "D3DManagement.h"
#include "fxManagement.h"
#include "ViewportManagement.h"
#include "SSManagement.h"
#include "RSManagement.h"
#include "gBuffer.h"
#include "renderingUtilities.h"
#include "d3dDebug.h"
#include "CBManagement.h"
#include "LightManagement.h"
#include "ModelManagement.h"
#include "TexManagement.h"
#include "ModelD3D.h"
#include "VB.h"
#include "IB.h"
#include "SubsetD3D.h"


#include "M3DLoader.h"
#include "AnimatedMesh.h"
#include "SkinnedData.h"

#include "renderingComponent.h"

RenderingComponent::RenderingComponent(HWND windowHandle)
{
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_POST_DESC_TEX);

	GET_ATTRIBUTES(cameraAttributes_, CameraAttribute, ATTRIBUTE_CAMERA);
	
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);

	windowHandle_	= windowHandle;
	screenWidth_	= windowResolution.width;
	screenHeight_	= windowResolution.height;
	numViewports_	= cameraAttributes_->size();
	csDispatchX_	= screenWidth_ / TILE_SIZE;
	csDispatchY_	= screenHeight_ / TILE_SIZE;

	d3dManagement_		= nullptr;
	fxManagement_		= nullptr;
	cbManagement_		= nullptr; 
	lightManagement_	= nullptr;
	modelManagement_	= nullptr;
	texManagement_		= nullptr;
	viewportManagement_ = nullptr;
	ssManagement_		= nullptr;
	rsManagement_		= nullptr;

	d3dDebug_	= nullptr;
	
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;

	m3dLoader_		= nullptr;
	animatedMesh_	= nullptr;
}
RenderingComponent::~RenderingComponent()
{
	SAFE_DELETE(d3dManagement_);
	SAFE_DELETE(fxManagement_);

	SAFE_DELETE(cbManagement_);
	SAFE_DELETE(lightManagement_);
	SAFE_DELETE(modelManagement_);
	SAFE_DELETE(texManagement_);
	SAFE_DELETE(viewportManagement_);
	SAFE_DELETE(ssManagement_);
	SAFE_DELETE(rsManagement_);
	
	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(d3dDebug_);

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);

	
	SAFE_DELETE(m3dLoader_);
	SAFE_DELETE(animatedMesh_);
}

void RenderingComponent::reset()
{
	if(d3dManagement_)
		d3dManagement_->reset();
	if(fxManagement_)
		fxManagement_->reset();
	if(cbManagement_)
		cbManagement_->reset();
	if(lightManagement_)
		lightManagement_->reset();
	if(viewportManagement_)
		viewportManagement_->reset();
	if(ssManagement_)
		ssManagement_->reset();
	if(rsManagement_)
		rsManagement_->reset();

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		if(gBuffers_[i])
			gBuffers_[i]->reset();
	

	EventManager::getInstance();
}

HRESULT RenderingComponent::resize(unsigned int screenWidth, unsigned int screenHeight)
{
	HRESULT hr = S_OK;

	screenWidth_	= screenWidth;	
	screenHeight_	= screenHeight;
	csDispatchX_	= screenWidth_ / TILE_SIZE;
	csDispatchY_	= screenHeight_ / TILE_SIZE;

	hr = d3dManagement_->resize(screenWidth, screenHeight);
	for(unsigned int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
	{
		if(SUCCEEDED(hr))
			hr = gBuffers_[i]->resize(d3dManagement_->getDevice(), screenWidth, screenHeight);
	}
	
	if(SUCCEEDED(hr))
		hr = viewportManagement_->resize(screenWidth, screenHeight);

	return hr;
}

HRESULT RenderingComponent::init()
{
	// init component
	//float* f = new float();
	HRESULT hr = S_OK;
	if(SUCCEEDED(hr))
		hr = initD3DManagement();
	if(SUCCEEDED(hr))
		hr = initFXManagement();
	if(SUCCEEDED(hr))
		hr = initCBManagement();
	if(SUCCEEDED(hr))
		hr = initLightManagement();
	if(SUCCEEDED(hr))
		hr = initModelManagement();
	if(SUCCEEDED(hr))
		hr = initTexManagement();
	if(SUCCEEDED(hr))
		hr = initViewport();
	if(SUCCEEDED(hr))
		hr = initSSManagement();
	if(SUCCEEDED(hr))
		hr = initRSManagement();
//	if(SUCCEEDED(hr))
//		hr = initDebug();
	if(SUCCEEDED(hr))
		hr = initGBuffers();

	//Removed in order to decrease startup time.
	//if(SUCCEEDED(hr))
	//	initAnimations();

	return hr;
}

void RenderingComponent::onUpdate(float delta)
{
	clearGBuffers();

	cbManagement_->vsSet(CB_TYPE_FRAME, CB_REGISTER_FRAME, d3dManagement_->getDeviceContext());
	cbManagement_->updateCBFrame(d3dManagement_->getDeviceContext(), lightManagement_->getNumLights());
	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		DirectX::XMFLOAT4X4 viewMatrix((float*)&cameraAttributes_->at(i).mat_view);
		DirectX::XMFLOAT4X4 projectionMatrix((float*)&cameraAttributes_->at(i).mat_projection);

		DirectX::XMFLOAT4X4 viewMatrixInverse		= calculateMatrixInverse(viewMatrix);
		DirectX::XMFLOAT4X4 projectionMatrixInverse	= calculateMatrixInverse(projectionMatrix);
		
		CameraAttribute* cameraAttribute = &cameraAttributes_->at(i);
		SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, cameraAttribute);
		PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatialAttribute);

		DirectX::XMFLOAT3	eyePosition = *(DirectX::XMFLOAT3*)&positionAttribute->position;
		
		unsigned int viewportTopX = static_cast<unsigned int>(viewportManagement_->getViewport(i).TopLeftX);
		unsigned int viewportTopY = static_cast<unsigned int>(viewportManagement_->getViewport(i).TopLeftY);
		cbManagement_->vsSet(CB_TYPE_CAMERA, CB_REGISTER_CAMERA, d3dManagement_->getDeviceContext());
		cbManagement_->updateCBCamera(d3dManagement_->getDeviceContext(),
									  viewMatrix,
									  viewMatrixInverse,
									  projectionMatrix,
									  projectionMatrixInverse,
									  eyePosition,
									  viewportTopX,
									  viewportTopY);

		setViewport(i);

		renderViewportToGBuffer(viewMatrix, projectionMatrix);
		renderViewportToBackBuffer();
	}
}
void RenderingComponent::renderViewportToGBuffer(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)									
{
	ID3D11Device*			device = d3dManagement_->getDevice();
	ID3D11DeviceContext*	devcon = d3dManagement_->getDeviceContext();

	d3dManagement_->clearDepthBuffer();

	if(animatedMesh_)
		renderAnimatedMesh(viewMatrix, projectionMatrix);

	fxManagement_->getDefaultVS()->set(d3dManagement_->getDeviceContext());
	fxManagement_->getDefaultPS()->set(d3dManagement_->getDeviceContext());
	ssManagement_->setPS(d3dManagement_->getDeviceContext(), SS_ID_DEFAULT, 0);
	rsManagement_->setRS(d3dManagement_->getDeviceContext(), RS_ID_DEFAULT);

	renderGBufferSetRenderTargets();

	// Fetch attributes
	std::vector<int>*				renderOwners;	GET_ATTRIBUTE_OWNERS(renderOwners,	ATTRIBUTE_RENDER);
	std::vector<RenderAttribute>*	allRender;		GET_ATTRIBUTES(allRender,	RenderAttribute,	ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>*	allSpatial;		GET_ATTRIBUTES(allSpatial,	SpatialAttribute,	ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>*	allPosition;	GET_ATTRIBUTES(allPosition,	PositionAttribute,	ATTRIBUTE_POSITION);
	
	DirectX::XMFLOAT4X4 worldMatrix;  
	DirectX::XMFLOAT4X4 worldMatrixInverse;
	DirectX::XMFLOAT4X4 finalMatrix;
	
	unsigned int meshID; ModelD3D* modelD3D;
	RenderAttribute* renderAt; SpatialAttribute* spatialAt; PositionAttribute* positionAt;
	for(unsigned int i=0; i<allRender->size(); i++)
	{
		if(renderOwners->at(i)!=0)
		{
			renderAt	= &allRender->at(i);
			meshID		= renderAt->meshID;
			spatialAt	= &allSpatial->at(renderAt->spatialAttribute.index);
			positionAt	= &allPosition->at(spatialAt->positionAttribute.index);
			
			modelD3D = modelManagement_->getModelD3D(meshID, d3dManagement_->getDevice());
			std::vector<MeshMaterial> materials = modelD3D->getMaterials();

			worldMatrix			= calculateWorldMatrix(spatialAt, positionAt);
			worldMatrixInverse	= calculateMatrixInverse(worldMatrix);
			finalMatrix			= calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
			
			cbManagement_->vsSet(CB_TYPE_OBJECT, CB_REGISTER_OBJECT, devcon);
			cbManagement_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
			VB* vb	= modelD3D->getVertexBuffer();
			std::vector<SubsetD3D*> subsetD3Ds = modelD3D->getSubsetD3Ds();

			UINT stride = sizeof(VertexPosNormTex);
			UINT offset = 0;
			
			ID3D11Buffer* vertexBuffer = vb->getVB();
			devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	
			for(unsigned int j = 0; j < subsetD3Ds.size(); j++)
			{
				ID3D11Buffer* indexBuffer = subsetD3Ds[j]->getIndexBuffer()->getIB();
				unsigned int materialIndex = subsetD3Ds[j]->getMaterialIndex();

				MeshMaterial material = materials[materialIndex];

				ID3D11ShaderResourceView* texAlbedo = texManagement_->getTexSrv(material.getIDAlbedoTex());
				ID3D11ShaderResourceView* texNormal = texManagement_->getTexSrv(material.getIDNormalTex());
				devcon->PSSetShaderResources(0, 1, &texAlbedo);
				devcon->PSSetShaderResources(1, 1, &texNormal);

				cbManagement_->psSet(CB_TYPE_SUBSET, CB_REGISTER_SUBSET, devcon);

				DirectX::XMFLOAT3 dxSpec(1.0f, 1.0f, 1.0f); //((float*)&material.getSpecularTerm());
				cbManagement_->updateCBSubset(
					devcon,
					dxSpec,
					material.getSpecularPower());

				devcon->IASetIndexBuffer(
					indexBuffer, 
					DXGI_FORMAT_R32_UINT, 
					offset);
	
				devcon->IASetInputLayout(fxManagement_->getILDefaultVSPosNormTex());
				devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				devcon->DrawIndexed(
					subsetD3Ds[j]->getIndexBuffer()->getNumIndices(), 
					0, 
					0);
			}
		}
	}

	renderGBufferClean();
}
void RenderingComponent::renderViewportToBackBuffer()
{
	d3dManagement_->setUAVBackBufferCS();

	cbManagement_->csSet(CB_TYPE_FRAME,		CB_REGISTER_FRAME,		d3dManagement_->getDeviceContext());
	cbManagement_->csSet(CB_TYPE_INSTANCE,	CB_REGISTER_INSTANCE,	d3dManagement_->getDeviceContext());
	cbManagement_->csSet(CB_TYPE_CAMERA,	CB_REGISTER_CAMERA,		d3dManagement_->getDeviceContext());
	cbManagement_->updateCBInstance(d3dManagement_->getDeviceContext(), screenWidth_, screenHeight_);

	lightManagement_->setLightSRVCS(d3dManagement_->getDeviceContext(), 3);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();
	d3dManagement_->getDeviceContext()->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	
	ssManagement_->setCS(d3dManagement_->getDeviceContext(), SS_ID_DEFAULT, 0);

	fxManagement_->getDefaultCS()->set(d3dManagement_->getDeviceContext());

	unsigned int dispatchX = csDispatchX_ / viewportManagement_->getNumViewportsX();
	unsigned int dispatchY = csDispatchY_ / viewportManagement_->getNumViewportsY();

	d3dManagement_->getDeviceContext()->Dispatch(dispatchX, dispatchY, 1);
	fxManagement_->getDefaultCS()->unset(d3dManagement_->getDeviceContext());

	renderBackBufferClean();

	d3dManagement_->present();
}

void RenderingComponent::setViewport(unsigned int index)
{
	viewportManagement_->setViewport(d3dManagement_->getDeviceContext(), index);
}
void RenderingComponent::clearGBuffers()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	
	d3dManagement_->getDeviceContext()->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO], black);
	d3dManagement_->getDeviceContext()->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL], black);
	d3dManagement_->getDeviceContext()->ClearRenderTargetView(renderTargets[GBUFFERID_MATERIAL], black);
}
void RenderingComponent::renderGBufferClean()
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();

	d3dManagement_->getDeviceContext()->VSSetShader(NULL, NULL, 0);
	d3dManagement_->getDeviceContext()->PSSetShader(NULL, NULL, 0);
	d3dManagement_->getDeviceContext()->CSSetShader(NULL, NULL, 0);
	
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = nullptr;
	
	d3dManagement_->getDeviceContext()->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, nullptr);
	d3dManagement_->getDeviceContext()->PSSetSamplers(0, 0, nullptr);
	d3dManagement_->getDeviceContext()->IASetInputLayout(nullptr);
	d3dManagement_->getDeviceContext()->RSSetState(nullptr);
}
void RenderingComponent::renderGBufferSetRenderTargets()
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	d3dManagement_->getDeviceContext()->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, d3dManagement_->getDepthBuffer());
}
void RenderingComponent::renderBackBufferClean()
{
	ID3D11UnorderedAccessView* uav2[] = { nullptr };
	d3dManagement_->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, uav2, NULL);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = nullptr;
	d3dManagement_->getDeviceContext()->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	d3dManagement_->getDeviceContext()->CSSetSamplers(0, 0, nullptr);
}

DirectX::XMFLOAT4X4 RenderingComponent::calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix)
{
	DirectX::CXMMATRIX	cxmMatrix		= DirectX::XMLoadFloat4x4(&matrix);
	DirectX::XMVECTOR	vDeterminant	= DirectX::XMMatrixDeterminant(cxmMatrix);
	DirectX::XMMATRIX	xmMatrixInverse = DirectX::XMMatrixInverse(&vDeterminant, cxmMatrix);
	
	DirectX::XMFLOAT4X4 matrixInverse;
	DirectX::XMStoreFloat4x4(&matrixInverse, xmMatrixInverse);
	
	return matrixInverse;
}
DirectX::XMFLOAT4X4 RenderingComponent::calculateWorldMatrix(SpatialAttribute* spatialAttribute, PositionAttribute* positionAttribute)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(positionAttribute->position.x,
																 positionAttribute->position.y,
																 positionAttribute->position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(spatialAttribute->scale.x,
														 spatialAttribute->scale.y,
														 spatialAttribute->scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(spatialAttribute->rotation.x,
													spatialAttribute->rotation.y,
													spatialAttribute->rotation.z,
													spatialAttribute->rotation.w);

	DirectX::XMVECTOR qRotation = DirectX::XMLoadFloat4(&fRotation);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(qRotation);

	DirectX::XMMATRIX mWorldMatrix = scaling * rotation * translation;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMStoreFloat4x4(&worldMatrix, mWorldMatrix);

	return worldMatrix;
}
DirectX::XMFLOAT4X4 RenderingComponent::calculateFinalMatrix(DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::CXMMATRIX mView		= DirectX::XMLoadFloat4x4(&viewMatrix);
	DirectX::CXMMATRIX mProjection	= DirectX::XMLoadFloat4x4(&projectionMatrix);
	DirectX::CXMMATRIX mWorld		= DirectX::XMLoadFloat4x4(&worldMatrix);

	DirectX::XMMATRIX mFinalMatrix = mWorld * mView * mProjection;
	
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, mFinalMatrix);

	return finalMatrix;
}

HRESULT RenderingComponent::initD3DManagement()
{
	HRESULT hr;
	d3dManagement_ = new D3DManagement(windowHandle_, screenWidth_, screenHeight_);
	hr = d3dManagement_->init();
	return hr;
}
HRESULT RenderingComponent::initFXManagement()
{
	HRESULT hr = S_OK;

	bool debugShaders = false;
#if defined (_DEBUG) || defined (DEBUG)
	debugShaders = true;
#endif //_DEBUG || DEBUG

	fxManagement_ = new FXManagement(debugShaders);
	hr = fxManagement_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initCBManagement()
{
	HRESULT hr = S_OK;

	cbManagement_ = new CBManagement();
	cbManagement_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initLightManagement()
{
	HRESULT hr = S_OK;

	lightManagement_ = new LightManagement();
	hr = lightManagement_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initModelManagement()
{
	HRESULT hr = S_OK;

	modelManagement_ = new ModelManagement();
	hr = modelManagement_->init();

	return hr;
}
HRESULT RenderingComponent::initTexManagement()
{
	HRESULT hr = S_OK;

	texManagement_ = new TexManagement();
	hr = texManagement_->init();

	return hr;
}
HRESULT RenderingComponent::initViewport()
{
	HRESULT hr = S_OK;
	viewportManagement_ = new ViewportManagement(numViewports_,
												 screenWidth_,
												 screenHeight_);
	hr = viewportManagement_->init();

	return hr;
}
HRESULT RenderingComponent::initSSManagement()
{
	HRESULT hr = S_OK;

	ssManagement_ = new SSManagement();
	hr = ssManagement_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initRSManagement()
{
	HRESULT hr = S_OK;

	rsManagement_ = new RSManagement();
	rsManagement_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initDebug()
{
	HRESULT hr = S_OK;

	d3dDebug_ = new D3DDebug();
	hr = d3dDebug_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initGBuffers()
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;

	/*Albedo*/
	gBuffer = new GBuffer(screenWidth_, screenHeight_, MULTISAMPLES_GBUFFERS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	hr = gBuffer->init(d3dManagement_->getDevice());
	gBuffers_[GBUFFERID_ALBEDO] = gBuffer;

	/*Normals*/
	if(SUCCEEDED(hr))
	{
		gBuffer = new GBuffer(screenWidth_, screenHeight_, MULTISAMPLES_GBUFFERS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		hr = gBuffer->init(d3dManagement_->getDevice());
		gBuffers_[GBUFFERID_NORMAL] = gBuffer;
	}

	/*Material*/
	if(SUCCEEDED(hr))
	{
		gBuffer = new GBuffer(screenWidth_, screenHeight_, MULTISAMPLES_GBUFFERS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		hr = gBuffer->init(d3dManagement_->getDevice());
		gBuffers_[GBUFFERID_MATERIAL] = gBuffer;
	}

	return hr;
}
void RenderingComponent::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_RESIZE:
		event_WindowResize((Event_WindowResize*)e);
		break;
	case EVENT_POST_DESC_TEX:
		event_PostDescTex((Event_PostDescTex*)e);
		break;
	default:
		break;
	}
}

void RenderingComponent::event_WindowResize( Event_WindowResize* e )
{
	int width = e->width;
	int height = e->height;

	resize(width, height);

	// TODO: resize render window
}

void RenderingComponent::event_PostDescTex(Event_PostDescTex* e)
{
	TexDesc* texDesc = e->texDesc_;

	texManagement_->handleTexDesc(
		texDesc,
		d3dManagement_->getDevice());

	delete texDesc;
}

void RenderingComponent::initAnimations()
{
	m3dLoader_ = new M3DLoader();
	
	animatedMesh_ = nullptr;
	animatedMesh_ = new AnimatedMesh();

	m3dLoader_->loadM3D("../../xkill-resources/xkill-models/soldier.m3d",
					   animatedMesh_->getVertices(),
					   animatedMesh_->getIndices(),
					   animatedMesh_->getSubsets(),
					   animatedMesh_->getMaterials(),
					   animatedMesh_->getSkinInfo());

	animatedMesh_->init(d3dManagement_->getDevice());
}

void RenderingComponent::renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	ID3D11Device*			device = d3dManagement_->getDevice();
	ID3D11DeviceContext*	devcon = d3dManagement_->getDeviceContext();

	DirectX::XMFLOAT4X4 worldMatrix(0.01f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.01f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.01f, 0.0f,
									10.0f, 2.3f, 1.0f, 1.0f);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
	DirectX::XMFLOAT4X4 finalMatrix			= calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	cbManagement_->vsSet(CB_TYPE_OBJECT, CB_REGISTER_OBJECT, devcon);
	cbManagement_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
	animatedMesh_->update(0.002f);
	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
	animatedMesh_->getSkinInfo()->getFinalTransforms("Take1", animatedMesh_->getTimePosition(), &finalTransforms);

	cbManagement_->vsSet(CB_TYPE_BONE, CB_REGISTER_BONE, devcon);
	cbManagement_->updateCBBone(devcon, finalTransforms);

	fxManagement_->getAnimationVS()->set(devcon);
	fxManagement_->getAnimationPS()->set(devcon);
	ssManagement_->setPS(d3dManagement_->getDeviceContext(), SS_ID_DEFAULT, 0);
	rsManagement_->setRS(d3dManagement_->getDeviceContext(), RS_ID_DEFAULT);

	renderGBufferSetRenderTargets();
	d3dManagement_->clearDepthBuffer();

	ID3D11Buffer* vertexBuffer = animatedMesh_->getVertexBuffer();
	UINT stride = sizeof(VertexPosNormTexTanSkinned);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	devcon->IASetIndexBuffer(animatedMesh_->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
	devcon->IASetInputLayout(fxManagement_->getILPosNormTexTanSkinned());
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(animatedMesh_->getNumIndices(), 0, 0);

	renderGBufferClean();
}
