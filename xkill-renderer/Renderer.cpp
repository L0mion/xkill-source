#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/MeshMaterial.h>

#include "ManagementD3D.h"
#include "ManagementFX.h"
#include "ManagementCB.h"
#include "ManagementLight.h"
#include "ManagementViewport.h"
#include "ManagementModel.h"
#include "ManagementTex.h"
#include "ManagementSS.h"
#include "ManagementRS.h"
#include "ManagementGBuffer.h"
#include "ManagementDebug.h"

#include "Winfo.h"
#include "gBuffer.h"
#include "renderingUtilities.h"
#include "Renderer.h"

//temp?
#include "ModelD3D.h"
#include "SubsetD3D.h"
#include "VB.h"
#include "IB.h"

//temp
#include "AnimatedMesh.h"
#include "M3DLoader.h"

Renderer::Renderer(HWND windowHandle)
{
	windowHandle_	= windowHandle;
	
	winfo_ = nullptr;

	managementD3D_		= nullptr;
	managementFX_		= nullptr;
	managementCB_		= nullptr;
	managementLight_	= nullptr;
	managementViewport_	= nullptr;
	managementModel_	= nullptr;
	managementTex_		= nullptr;
	managementSS_		= nullptr;
	managementRS_		= nullptr;
	managementGBuffer_	= nullptr;
	managementDebug_	= nullptr;

	attributesSpatial_		= nullptr;
	attributesPosition_		= nullptr;
	attributesRender_		= nullptr;
	attributesRenderOwner_	= nullptr;
	attributesCamera_		= nullptr;

	//temp
	m3dLoader_		= nullptr;
	animatedMesh_	= nullptr;
}
Renderer::~Renderer()	
{
	SAFE_DELETE(winfo_);

	SAFE_DELETE(managementD3D_);
	SAFE_DELETE(managementFX_);	
	SAFE_DELETE(managementCB_);
	SAFE_DELETE(managementLight_);
	SAFE_DELETE(managementViewport_);
	SAFE_DELETE(managementModel_);
	SAFE_DELETE(managementTex_);
	SAFE_DELETE(managementSS_);
	SAFE_DELETE(managementRS_);
	SAFE_DELETE(managementGBuffer_);

	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(managementDebug_);

	//temp
	SAFE_DELETE(m3dLoader_);
	SAFE_DELETE(animatedMesh_);
}

void Renderer::reset()
{
	SAFE_RESET(managementD3D_);
	SAFE_RESET(managementD3D_);
	SAFE_RESET(managementFX_);	
	SAFE_RESET(managementCB_);
	SAFE_RESET(managementLight_);
	SAFE_RESET(managementViewport_);
	//SAFE_RESET(managementModel_);		//?
	//SAFE_RESET(managementTex_);		//?
	SAFE_RESET(managementSS_);
	SAFE_RESET(managementRS_);
	SAFE_RESET(managementGBuffer_);
}
HRESULT Renderer::resize(unsigned int screenWidth, unsigned int screenHeight)
{
	HRESULT hr = S_OK;

	unsigned int numViewports, csDispatchX, csDispatchY;
	numViewports	= winfo_->getNumViewports();
	csDispatchX		= screenWidth	/ CS_TILE_SIZE;
	csDispatchY		= screenHeight	/ CS_TILE_SIZE;
	winfo_->init(
		screenWidth, 
		screenHeight, 
		numViewports, 
		csDispatchX, 
		csDispatchY);

	hr = managementD3D_->resize();
	if(SUCCEEDED(hr))
		hr = managementGBuffer_->resize(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = managementViewport_->resize();

	return hr;
}

HRESULT Renderer::init()
{
	HRESULT hr = S_OK;

	//call me
	initAttributes();
	initWinfo();
	if(SUCCEEDED(hr))
		hr = initManagementD3D();
	if(SUCCEEDED(hr))
		hr = initManagementFX();
	if(SUCCEEDED(hr))
		hr = initManagementCB();
	if(SUCCEEDED(hr))
		hr = initManagementLight();
	if(SUCCEEDED(hr))
		hr = initManagementModel();
	if(SUCCEEDED(hr))
		hr = initManagementTex();
	if(SUCCEEDED(hr))
		hr = initManagementViewport();
	if(SUCCEEDED(hr))
		hr = initManagementSS();
	if(SUCCEEDED(hr))
		hr = initManagementRS();
//	if(SUCCEEDED(hr))
//		hr = initManagementDebug();
	if(SUCCEEDED(hr))
		hr = initManagementGBuffer();

	//temp
	m3dLoader_ = new M3DLoader();
	animatedMesh_ = nullptr;
	animatedMesh_ = new AnimatedMesh();
	m3dLoader_->loadM3D("../../xkill-resources/xkill-models/soldier.m3d",
					   animatedMesh_->getVertices(),
					   animatedMesh_->getIndices(),
					   animatedMesh_->getSubsets(),
					   animatedMesh_->getMaterials(),
					   animatedMesh_->getSkinInfo());
	animatedMesh_->init(managementD3D_->getDevice());

	return hr;
}
void Renderer::initAttributes()
{	
	GET_ATTRIBUTES(attributesCamera_,	CameraAttribute,	ATTRIBUTE_CAMERA);
	GET_ATTRIBUTES(attributesRender_,	RenderAttribute,	ATTRIBUTE_RENDER);
	GET_ATTRIBUTES(attributesSpatial_,	SpatialAttribute,	ATTRIBUTE_SPATIAL);
	GET_ATTRIBUTES(attributesPosition_,	PositionAttribute,	ATTRIBUTE_POSITION);

	GET_ATTRIBUTE_OWNERS(attributesRenderOwner_, ATTRIBUTE_RENDER);
}
void Renderer::initWinfo()
{
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);

	unsigned int screenWidth, screenHeight, numViewports, csDispatchX, csDispatchY;
	screenWidth		= windowResolution.width;
	screenHeight	= windowResolution.height;
	numViewports	= attributesCamera_->size();
	csDispatchX		= screenWidth	/ CS_TILE_SIZE;
	csDispatchY		= screenHeight	/ CS_TILE_SIZE;

	winfo_ = new Winfo();
	winfo_->init(
		screenWidth, 
		screenHeight, 
		numViewports, 
		csDispatchX, 
		csDispatchY);
}
HRESULT Renderer::initManagementD3D()
{
	HRESULT hr;

	managementD3D_ = new ManagementD3D(windowHandle_, winfo_);
	hr = managementD3D_->init();

	return hr;
}
HRESULT Renderer::initManagementFX()
{
	HRESULT hr = S_OK;

	bool debugShaders = false;
#if defined (_DEBUG) || defined (DEBUG)
	debugShaders = true;
#endif //_DEBUG || DEBUG

	managementFX_ = new ManagementFX(debugShaders);
	hr = managementFX_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementCB()
{
	HRESULT hr = S_OK;

	managementCB_ = new ManagementCB();
	managementCB_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementLight()
{
	HRESULT hr = S_OK;

	managementLight_ = new ManagementLight();
	hr = managementLight_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementModel()
{
	HRESULT hr = S_OK;

	managementModel_ = new ManagementModel();
	hr = managementModel_->init();

	return hr;
}
HRESULT Renderer::initManagementTex()
{
	HRESULT hr = S_OK;

	managementTex_ = new ManagementTex();
	hr = managementTex_->init();

	return hr;
}
HRESULT Renderer::initManagementViewport()
{
	HRESULT hr = S_OK;

	managementViewport_ = new ManagementViewport(winfo_);
	hr = managementViewport_->init();

	return hr;
}
HRESULT Renderer::initManagementSS()
{
	HRESULT hr = S_OK;

	managementSS_ = new ManagementSS();
	hr = managementSS_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementRS()
{
	HRESULT hr = S_OK;

	managementRS_ = new ManagementRS();
	managementRS_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementGBuffer()
{
	HRESULT hr = S_OK;

	managementGBuffer_ = new ManagementGBuffer(winfo_);
	hr = managementGBuffer_->init(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementDebug()
{
	HRESULT hr = S_OK;

	managementDebug_ = new ManagementDebug();
	hr = managementDebug_->init(managementD3D_->getDevice());

	return hr;
}

void Renderer::render(float delta)
{
	managementGBuffer_->clearGBuffers(managementD3D_->getDeviceContext());

	managementCB_->vsSet(CB_TYPE_FRAME, CB_REGISTER_FRAME, managementD3D_->getDeviceContext());
	managementCB_->updateCBFrame(managementD3D_->getDeviceContext(), managementLight_->getNumLights());
	for(unsigned int i = 0; i < attributesCamera_->size(); i++)
	{
		DirectX::XMFLOAT4X4 viewMatrix((float*)&attributesCamera_->at(i).mat_view);
		DirectX::XMFLOAT4X4 projectionMatrix((float*)&attributesCamera_->at(i).mat_projection);

		DirectX::XMFLOAT4X4 viewMatrixInverse		= calculateMatrixInverse(viewMatrix);
		DirectX::XMFLOAT4X4 projectionMatrixInverse	= calculateMatrixInverse(projectionMatrix);
		
		CameraAttribute* cameraAttribute		= &attributesCamera_->at(i);
		SpatialAttribute* spatialAttribute		= ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, cameraAttribute);
		PositionAttribute* positionAttribute	= ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatialAttribute);

		DirectX::XMFLOAT3	eyePosition = *(DirectX::XMFLOAT3*)&positionAttribute->position;
		
		unsigned int viewportTopX = static_cast<unsigned int>(managementViewport_->getViewport(i).TopLeftX);
		unsigned int viewportTopY = static_cast<unsigned int>(managementViewport_->getViewport(i).TopLeftY);
		managementCB_->vsSet(CB_TYPE_CAMERA, CB_REGISTER_CAMERA, managementD3D_->getDeviceContext());
		managementCB_->updateCBCamera(managementD3D_->getDeviceContext(),
									  viewMatrix,
									  viewMatrixInverse,
									  projectionMatrix,
									  projectionMatrixInverse,
									  eyePosition,
									  viewportTopX,
									  viewportTopY);

		managementViewport_->setViewport(managementD3D_->getDeviceContext(), i);

		renderViewportToGBuffer(viewMatrix, projectionMatrix);
		renderViewportToBackBuffer();
	}
}

void Renderer::renderViewportToGBuffer(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)									
{
	//temp
	GET_ATTRIBUTES(attributesCamera_,	CameraAttribute,	ATTRIBUTE_CAMERA);
	GET_ATTRIBUTES(attributesRender_,	RenderAttribute,	ATTRIBUTE_RENDER);
	GET_ATTRIBUTES(attributesSpatial_,	SpatialAttribute,	ATTRIBUTE_SPATIAL);
	GET_ATTRIBUTES(attributesPosition_,	PositionAttribute,	ATTRIBUTE_POSITION);
	GET_ATTRIBUTE_OWNERS(attributesRenderOwner_, ATTRIBUTE_RENDER);

	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	managementD3D_->clearDepthBuffer();

	if(animatedMesh_)
		renderAnimatedMesh(viewMatrix, projectionMatrix);

	managementFX_->getDefaultVS()->set(devcon);
	managementFX_->getDefaultPS()->set(devcon);
	managementSS_->setPS(devcon, SS_ID_DEFAULT, 0);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBufferAsRenderTargets(devcon, managementD3D_->getDepthBuffer());

	unsigned int		meshID;
	ModelD3D*			modelD3D;
	RenderAttribute*	renderAt;
	SpatialAttribute*	spatialAt;
	PositionAttribute*	positionAt;
	DirectX::XMFLOAT4X4 worldMatrix;  
	DirectX::XMFLOAT4X4 worldMatrixInverse;
	DirectX::XMFLOAT4X4 finalMatrix;
	for(unsigned int i = 0; i < attributesRender_->size() && attributesRenderOwner_->at(i) != 0; i++)
	{
		renderAt	= &attributesRender_->at(i);
		spatialAt	= &attributesSpatial_->at(renderAt->spatialAttribute.index);
		positionAt	= &attributesPosition_->at(spatialAt->positionAttribute.index);
		
		meshID		= renderAt->meshID;
		modelD3D	= managementModel_->getModelD3D(meshID, device);
		std::vector<MeshMaterial> materials = modelD3D->getMaterials();

		worldMatrix			= calculateWorldMatrix(spatialAt, positionAt);
		worldMatrixInverse	= calculateMatrixInverse(worldMatrix);
		finalMatrix			= calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
		
		managementCB_->vsSet(CB_TYPE_OBJECT, CB_REGISTER_OBJECT, devcon);
		managementCB_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
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

			ID3D11ShaderResourceView* texAlbedo = managementTex_->getTexSrv(material.getIDAlbedoTex());
			ID3D11ShaderResourceView* texNormal = managementTex_->getTexSrv(material.getIDNormalTex());
			devcon->PSSetShaderResources(0, 1, &texAlbedo);
			devcon->PSSetShaderResources(1, 1, &texNormal);

			managementCB_->psSet(CB_TYPE_SUBSET, CB_REGISTER_SUBSET, devcon);

			DirectX::XMFLOAT3 dxSpec(1.0f, 1.0f, 1.0f); //((float*)&material.getSpecularTerm());
			managementCB_->updateCBSubset(
				devcon,
				dxSpec,
				material.getSpecularPower());

			devcon->IASetIndexBuffer(
				indexBuffer, 
				DXGI_FORMAT_R32_UINT, 
				offset);
	
			devcon->IASetInputLayout(managementFX_->getILDefaultVSPosNormTex());
			devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			devcon->DrawIndexed(
				subsetD3Ds[j]->getIndexBuffer()->getNumIndices(), 
				0, 
				0);
		}
	}

	renderGBufferClean();
}
void Renderer::renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	DirectX::XMFLOAT4X4 worldMatrix(0.01f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.01f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.01f, 0.0f,
									10.0f, 2.3f, 1.0f, 1.0f);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
	DirectX::XMFLOAT4X4 finalMatrix			= calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	managementCB_->vsSet(CB_TYPE_OBJECT, CB_REGISTER_OBJECT, devcon);
	managementCB_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
	animatedMesh_->update(0.002f);
	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
	animatedMesh_->getSkinInfo()->getFinalTransforms("Take1", animatedMesh_->getTimePosition(), &finalTransforms);

	managementCB_->vsSet(CB_TYPE_BONE, CB_REGISTER_BONE, devcon);
	managementCB_->updateCBBone(devcon, finalTransforms);

	managementFX_->getAnimationVS()->set(devcon);
	managementFX_->getAnimationPS()->set(devcon);
	managementSS_->setPS(devcon, SS_ID_DEFAULT, 0);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBufferAsRenderTargets(devcon, managementD3D_->getDepthBuffer());

	managementD3D_->clearDepthBuffer();

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
	
	devcon->IASetInputLayout(managementFX_->getILPosNormTexTanSkinned());
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(animatedMesh_->getNumIndices(), 0, 0);

	renderGBufferClean();
}
void Renderer::renderViewportToBackBuffer()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	managementD3D_->setUAVBackBufferCS();

	managementCB_->csSet(CB_TYPE_FRAME,		CB_REGISTER_FRAME,		devcon);
	managementCB_->csSet(CB_TYPE_INSTANCE,	CB_REGISTER_INSTANCE,	devcon);
	managementCB_->csSet(CB_TYPE_CAMERA,	CB_REGISTER_CAMERA,		devcon);
	managementCB_->updateCBInstance(devcon, winfo_->getScreenWidth(), winfo_->getScreenHeight());

	managementLight_->setLightSRVCS(devcon, 3);

	managementGBuffer_->setGBuffersAsCSShaderResources(devcon);
	
	managementSS_->setCS(devcon, SS_ID_DEFAULT, 0);

	managementFX_->getDefaultCS()->set(devcon);

	unsigned int dispatchX = winfo_->getCSDispathX() / managementViewport_->getNumViewportsX();
	unsigned int dispatchY = winfo_->getCSDispathY() / managementViewport_->getNumViewportsY();

	devcon->Dispatch(dispatchX, dispatchY, 1);
	managementFX_->getDefaultCS()->unset(devcon);

	renderBackBufferClean();

	managementD3D_->present();
}
void Renderer::renderGBufferClean()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	managementGBuffer_->unsetGBuffersAndDepthBufferAsRenderTargets(devcon);

	//put me into managementfx?
	devcon->VSSetShader(NULL, NULL, 0);
	devcon->PSSetShader(NULL, NULL, 0);
	devcon->CSSetShader(NULL, NULL, 0);

	devcon->PSSetSamplers(0, 0, nullptr);
	devcon->IASetInputLayout(nullptr);
	devcon->RSSetState(nullptr);
}
void Renderer::renderBackBufferClean()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	ID3D11UnorderedAccessView* uav2[] = { nullptr };
	devcon->CSSetUnorderedAccessViews(0, 1, uav2, NULL);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = nullptr;
	devcon->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	devcon->CSSetSamplers(0, 0, nullptr);
}

DirectX::XMFLOAT4X4 Renderer::calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix)
{
	DirectX::CXMMATRIX	cxmMatrix		= DirectX::XMLoadFloat4x4(&matrix);
	DirectX::XMVECTOR	vDeterminant	= DirectX::XMMatrixDeterminant(cxmMatrix);
	DirectX::XMMATRIX	xmMatrixInverse = DirectX::XMMatrixInverse(&vDeterminant, cxmMatrix);
	
	DirectX::XMFLOAT4X4 matrixInverse;
	DirectX::XMStoreFloat4x4(&matrixInverse, xmMatrixInverse);
	
	return matrixInverse;
}
DirectX::XMFLOAT4X4 Renderer::calculateWorldMatrix(
	SpatialAttribute* spatialAttribute, 
	PositionAttribute* positionAttribute)
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
DirectX::XMFLOAT4X4 Renderer::calculateFinalMatrix(
	DirectX::XMFLOAT4X4 worldMatrix, 
	DirectX::XMFLOAT4X4 viewMatrix, 
	DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::CXMMATRIX mView		= DirectX::XMLoadFloat4x4(&viewMatrix);
	DirectX::CXMMATRIX mProjection	= DirectX::XMLoadFloat4x4(&projectionMatrix);
	DirectX::CXMMATRIX mWorld		= DirectX::XMLoadFloat4x4(&worldMatrix);

	DirectX::XMMATRIX mFinalMatrix = mWorld * mView * mProjection;
	
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, mFinalMatrix);

	return finalMatrix;
}

void Renderer::loadTextures(TexDesc* texdesc)
{
	managementTex_->handleTexDesc(texdesc, managementD3D_->getDevice());
}