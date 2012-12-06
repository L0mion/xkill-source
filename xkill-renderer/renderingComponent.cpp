#include "renderingComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>
#include <xkill-utilities/MeshModel.h>

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
#include "MeshManagement.h"
#include "MeshModelD3D.h"
#include "VB.h"
#include "IB.h"

#include "renderingComponent.h"

#include <iostream>

RenderingComponent::RenderingComponent(HWND windowHandle)
{
	GET_ATTRIBUTES(cameraAttributes_, CameraAttribute, ATTRIBUTE_CAMERA);
	
	Event_getWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);

	windowHandle_	= windowHandle;
	screenWidth_	= windowResolution.width;
	screenHeight_	= windowResolution.height;
	numViewports_	= cameraAttributes_->size();

	d3dManagement_		= nullptr;
	fxManagement_		= nullptr;
	cbManagement_		= nullptr; 
	lightManagement_	= nullptr;
	meshManagement_		= nullptr;
	viewportManagement_ = nullptr;
	ssManagement_		= nullptr;
	rsManagement_		= nullptr;

	d3dDebug_	= nullptr;
	
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;

	float* temp = new float;
}
RenderingComponent::~RenderingComponent()
{
	SAFE_DELETE(d3dManagement_);
	SAFE_DELETE(fxManagement_);

	SAFE_DELETE(cbManagement_);
	SAFE_DELETE(lightManagement_);
	SAFE_DELETE(meshManagement_);
	SAFE_DELETE(viewportManagement_);
	SAFE_DELETE(ssManagement_);
	SAFE_DELETE(rsManagement_);
	
	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(d3dDebug_);

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);
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

	d3dManagement_->getDeviceContext()->RSSetViewports(0, 0);

	for(unsigned int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
	{
		gBuffers_[i]->reset();
	}

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
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);

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
		hr = initMeshManagement();
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

	return hr;
}

void RenderingComponent::onUpdate(float delta)
{
	clearGBuffers();
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

		cbManagement_->vsSet(CB_FRAME_INDEX, 0, d3dManagement_->getDeviceContext());
		cbManagement_->updateCBFrame(d3dManagement_->getDeviceContext(),
									 viewMatrix,
									 viewMatrixInverse,
									 projectionMatrix,
									 projectionMatrixInverse,
									 eyePosition, 
									 lightManagement_->getNumLights());

		setViewport(i);
		renderToGBuffer(viewMatrix, projectionMatrix);
	}
	
	renderToBackBuffer();
}
void RenderingComponent::renderToGBuffer(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)									
{
	ID3D11Device*			device = d3dManagement_->getDevice();
	ID3D11DeviceContext*	devcon = d3dManagement_->getDeviceContext();

	fxManagement_->getDefaultVS()->set(d3dManagement_->getDeviceContext());
	fxManagement_->getDefaultPS()->set(d3dManagement_->getDeviceContext());
	ssManagement_->setPS(d3dManagement_->getDeviceContext(), SS_ID_DEFAULT, 0);
	rsManagement_->setRS(d3dManagement_->getDeviceContext(), RS_ID_DEFAULT);

	renderGBufferSetRenderTargets();
	
	d3dManagement_->clearDepthBuffer();

	// Fetch attributes
	std::vector<RenderAttribute>*	allRender;		GET_ATTRIBUTES(allRender,	RenderAttribute,	ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>*	allSpatial;		GET_ATTRIBUTES(allSpatial,	SpatialAttribute,	ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>*	allPosition;	GET_ATTRIBUTES(allPosition,	PositionAttribute,	ATTRIBUTE_POSITION);
	
	DirectX::XMFLOAT4X4 worldMatrix;  
	DirectX::XMFLOAT4X4 worldMatrixInverse;
	DirectX::XMFLOAT4X4 finalMatrix;
	
	unsigned int meshIndex; MeshModelD3D* meshModelD3D;
	RenderAttribute* renderAt; SpatialAttribute* spatialAt; PositionAttribute* positionAt;
	for(unsigned int i=0; i<allRender->size(); i++)
	{
		renderAt	= &allRender->at(i);
		meshIndex	= renderAt->meshIndex;
		spatialAt	= &allSpatial->at(renderAt->spatialAttribute.index);
		positionAt	= &allPosition->at(spatialAt->positionAttribute.index);
		
		meshModelD3D = meshManagement_->getMeshModelD3D(meshIndex, d3dManagement_->getDevice());
		VB*					vb	= meshModelD3D->getVB();
		std::vector<IB*>	ibs	= meshModelD3D->getIBs();

		worldMatrix			= calculateWorldMatrix(allSpatial->at(i), allPosition->at(i));
		worldMatrixInverse	= calculateMatrixInverse(worldMatrix);
		finalMatrix			= calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
		
		cbManagement_->vsSet(CB_OBJECT_INDEX, 2, devcon);
		cbManagement_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);

		UINT stride = sizeof(VertexPosNormTex);
		UINT offset = 0;
		
		ID3D11Buffer* vertexBuffer = vb->getVB();
		devcon->IASetVertexBuffers(
			0, 
			1, 
			&vertexBuffer, 
			&stride, 
			&offset);

		for(unsigned int i = 0; i < ibs.size(); i++)
		{
			devcon->IASetIndexBuffer(ibs[i]->getIB(), DXGI_FORMAT_R32_UINT, offset);

			devcon->IASetInputLayout(fxManagement_->getILDefaultVSPosNormTex());
			devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			devcon->DrawIndexed(ibs[i]->getNumIndices(), 0, 0);
		}
	}

	renderGBufferClean();
}
void RenderingComponent::renderToBackBuffer()
{
	d3dManagement_->setUAVBackBufferCS();

	cbManagement_->csSet(CB_FRAME_INDEX, 0, d3dManagement_->getDeviceContext());
	cbManagement_->csSet(CB_INSTANCE_INDEX, 1, d3dManagement_->getDeviceContext());
	cbManagement_->updateCBInstance(d3dManagement_->getDeviceContext(), screenWidth_, screenHeight_);

	lightManagement_->setLightSRVCS(d3dManagement_->getDeviceContext(), 2);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();
	d3dManagement_->getDeviceContext()->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	
	ssManagement_->setCS(d3dManagement_->getDeviceContext(), SS_ID_DEFAULT, 0);

	fxManagement_->getDefaultCS()->set(d3dManagement_->getDeviceContext());
	d3dManagement_->getDeviceContext()->Dispatch(25, 25, 1);
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
DirectX::XMFLOAT4X4 RenderingComponent::calculateWorldMatrix(SpatialAttribute spatialAttribute, PositionAttribute positionAttribute)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(positionAttribute.position.x,
																 positionAttribute.position.y,
																 positionAttribute.position.z);

	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(spatialAttribute.scale.x,
														 spatialAttribute.scale.y,
														 spatialAttribute.scale.z);

	DirectX::XMFLOAT4 fRotation = DirectX::XMFLOAT4(spatialAttribute.rotation.x,
													spatialAttribute.rotation.y,
													spatialAttribute.rotation.z,
													spatialAttribute.rotation.w);

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

	fxManagement_ = new FXManagement();
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
HRESULT RenderingComponent::initMeshManagement()
{
	HRESULT hr = S_OK;

	meshManagement_ = new MeshManagement();

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
	if(hr == S_OK)
	{
		gBuffer = new GBuffer(screenWidth_, screenHeight_, MULTISAMPLES_GBUFFERS, DXGI_FORMAT_R32G32B32A32_FLOAT);
		hr = gBuffer->init(d3dManagement_->getDevice());
		gBuffers_[GBUFFERID_NORMAL] = gBuffer;
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
