#include <xkill-utilities/Util.h>
#include <DirectXMath.h>

#include "ManagementD3D.h"
#include "ManagementFX.h"
#include "ManagementCB.h"
#include "ManagementLight.h"
#include "ManagementViewport.h"
#include "ManagementModel.h"
#include "ManagementTex.h"
#include "ManagementSS.h"
#include "ManagementRS.h"
#include "ManagementBuffer.h"
#include "ManagementDebug.h"
#include "ManagementMath.h"
#include "ManagementInstance.h"
#include "ManagementSprites.h"

#include "Winfo.h"
#include "ModelD3D.h"
#include "Buffer_SrvRtv.h"
#include "SubsetD3D.h"
#include "DebugShapeD3D.h"
#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "TypeFX.h"
#include "Renderer.h"
#include "ViewportData.h"

ATTRIBUTES_DECLARE_ALL;

//temp
#include "Buffer_SrvRtvUav.h"

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
	managementBuffer_	= nullptr;
	managementDebug_	= nullptr;
	managementMath_		= nullptr;
	managementInstance_ = nullptr;
	managementSprites_  = nullptr;

	ATTRIBUTES_INIT_ALL;

	debugLinesVertexBuffer_ = nullptr;

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
	SAFE_DELETE(managementBuffer_);
	SAFE_DELETE(managementMath_);
	SAFE_DELETE(managementInstance_);
	SAFE_DELETE(managementSprites_);

	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(managementDebug_);

	SAFE_RELEASE(debugLinesVertexBuffer_);

	//temp
//	SAFE_DELETE(m3dLoader_);
//	SAFE_DELETE(animatedMesh_);
}

void Renderer::reset()
{
	SAFE_RESET(managementD3D_);
	SAFE_RESET(managementD3D_);
	SAFE_RESET(managementFX_);	
	SAFE_RESET(managementCB_);
	SAFE_RESET(managementLight_);
	SAFE_RESET(managementViewport_);
	SAFE_RESET(managementSS_);
	SAFE_RESET(managementRS_);
	SAFE_RESET(managementBuffer_);
	SAFE_RESET(managementSprites_);
}

void Renderer::unloadModels()
{
	managementModel_->unloadModels();
}

HRESULT Renderer::resize(unsigned int screenWidth, unsigned int screenHeight)
{
	//Get number of split-screens
	unsigned int numSS = itrSplitScreen.count();
	
	//Initialize new windo-type object.
	HRESULT hr = S_OK;
	unsigned int numViewports, csDispatchX, csDispatchY;
	numViewports	= numSS;

	csDispatchX	= ceil((float)screenWidth	/ (float)CS_TILE_SIZE);
	csDispatchY	= ceil((float)screenHeight	/ (float)CS_TILE_SIZE);
	winfo_->init(
		screenWidth, 
		screenHeight, 
		numViewports, 
		csDispatchX, 
		csDispatchY);

	hr = managementD3D_->resize();
	if(SUCCEEDED(hr))
		hr = managementBuffer_->resize(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = managementViewport_->resize();

	//Update per-instance constant-buffer.
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementCB_->setCB(
		CB_TYPE_INSTANCE,
		TypeFX_CS, 
		CB_REGISTER_INSTANCE, 
		devcon);
	managementCB_->updateCBInstance(
		devcon, 
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight());

	return hr;
}

HRESULT Renderer::init()
{
	HRESULT hr = S_OK;

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
	initManagementMath();
	if(SUCCEEDED(hr))
		hr = initManagementGBuffer();
	if(SUCCEEDED(hr))
		hr = initManagementSprites();
	initManagementInstance();

	initSSAO();

	//temp
	/*
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
	*/

	return hr;
}
void Renderer::initWinfo()
{
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);

	unsigned int screenWidth, screenHeight, numViewports, csDispatchX, csDispatchY;
	screenWidth		= windowResolution.width;
	screenHeight	= windowResolution.height;
	numViewports	= itrCamera.count();
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
	hr = managementCB_->init(managementD3D_->getDevice());

	if(SUCCEEDED(hr))
	{
		ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
		managementCB_->setCB(
			CB_TYPE_INSTANCE,
			TypeFX_CS, 
			CB_REGISTER_INSTANCE, 
			devcon);
		managementCB_->updateCBInstance(
			devcon, 
			winfo_->getScreenWidth(), 
			winfo_->getScreenHeight());
	}

	return hr;
}
HRESULT Renderer::initManagementLight()
{
	HRESULT hr = S_OK;

	managementLight_ = new ManagementLight();
	managementLight_->init();

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

	managementBuffer_ = new ManagementBuffer(winfo_);
	hr = managementBuffer_->init(managementD3D_->getDevice(), managementD3D_->getDeviceContext());

	return hr;
}
HRESULT Renderer::initManagementDebug()
{
	HRESULT hr = S_OK;

	managementDebug_ = new ManagementDebug();
	hr = managementDebug_->init(managementD3D_->getDevice());

	return hr;
}
void Renderer::initManagementMath()
{
	managementMath_ = new ManagementMath();
}
void Renderer::initManagementInstance()
{
	managementInstance_ = new ManagementInstance();
}
HRESULT Renderer::initManagementSprites()
{
	HRESULT hr = S_OK;
	managementSprites_ = new ManagementSprites();
	hr = managementSprites_->init(managementD3D_->getDevice());
	return hr;
}
void Renderer::initSSAO()
{
	buildOffsetKernel();
}

void Renderer::update()
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Update lights.
	managementLight_->update(device, devcon);

	//Update instances.
	managementInstance_->update(device, devcon);
}
void Renderer::render()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Clear g-buffers and depth buffer.
	managementBuffer_->clearBuffers(devcon);
	managementD3D_->clearDepthBuffer();
	managementD3D_->clearBackBuffer();

	//Do shadows pre-pass:
	DirectX::XMFLOAT4X4 shadowMapTransform = buildShadowMap();

	//Update per-frame constant buffer.
	managementCB_->setCB(
		CB_TYPE_FRAME, 
		TypeFX_VS, 
		CB_REGISTER_FRAME, 
		devcon);
	managementCB_->updateCBFrame(
		devcon,
		shadowMapTransform,
		managementLight_->getLightDirCurCount(),
		managementLight_->getLightPointCurCount(),
		managementLight_->getLightSpotCurCount());

	AttributePtr<Attribute_SplitScreen>	ptr_splitScreen;
	AttributePtr<Attribute_Camera>		ptr_camera; 
	AttributePtr<Attribute_Spatial>		ptr_spatial;
	AttributePtr<Attribute_Position>	ptr_position;

	ViewportData vpData;

	managementBuffer_->setBuffersAndDepthBufferAsRenderTargets(devcon, managementD3D_->getDepthBuffer());

	//Render each split-screen separately
	std::vector<SplitScreenViewport>* ssViewports = managementViewport_->getSplitScreenViewports();
	std::vector<ViewportData> vpDatas(ssViewports->size());
	for(unsigned int i = 0; i < ssViewports->size(); i++)
	{
		ptr_splitScreen		= ssViewports->at(i).ptr_splitScreen;
		ptr_camera			= ptr_splitScreen->ptr_camera;
		ptr_spatial			= ptr_camera->ptr_spatial;
		ptr_position		= ptr_spatial->ptr_position;

		managementViewport_->setViewport(devcon, i);

		//Store all the viewport-specific data for the backbuffer-rendering.
		vpData.camIndex		= ptr_camera.index();
		vpData.view			= DirectX::XMFLOAT4X4(((float*)&ptr_camera->mat_view));
		vpData.proj			= DirectX::XMFLOAT4X4(((float*)&ptr_camera->mat_projection));
		vpData.viewInv		= managementMath_->calculateMatrixInverse(vpData.view);
		vpData.projInv		= managementMath_->calculateMatrixInverse(vpData.proj);
		vpData.eyePos		= *(DirectX::XMFLOAT3*)&ptr_position->position;
		vpData.viewportTopX = static_cast<unsigned int>(ptr_splitScreen->ssTopLeftX);
		vpData.viewportTopY = static_cast<unsigned int>(ptr_splitScreen->ssTopLeftY);
		vpData.zNear		= ptr_camera->zNear;
		vpData.zFar			= ptr_camera->zFar;
		vpData.viewportWidth	= (float)ptr_splitScreen->ssWidth;
		vpData.viewportHeight	= (float)ptr_splitScreen->ssHeight;
		vpDatas[i]			= vpData;

		renderViewportToGBuffer(vpData);
	}
	managementBuffer_->unsetBuffersAndDepthBufferAsRenderTargets(devcon);

	//Blur glowmap:
	downSampleBlur();
	unsigned int numBlurs = 1;
	for(unsigned int i = 0; i < numBlurs; i++)
	{
		blurHorizontally();
		blurVertically();
	}
	upSampleBlur();

	//Compute SSAO for each viewport:
	for(unsigned int i = 0; i < vpDatas.size(); i++)
		buildSSAOMap(vpDatas[i]);

	//Render everything to backbuffer:
	for(unsigned int i = 0; i < vpDatas.size(); i++)
		renderViewportToBackBuffer(vpDatas[i]);

	for(unsigned int i=0; i< vpDatas.size(); i++)
		renderHudElements(i);

	managementD3D_->present();
}
void Renderer::renderViewportToGBuffer(ViewportData& vpData)									
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	if(animatedMesh_)
		renderAnimatedMesh(vpData.view, vpData.proj);

	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	//Update per-viewport constant buffer.
	managementCB_->setCB(CB_TYPE_CAMERA, TypeFX_VS, CB_REGISTER_CAMERA, managementD3D_->getDeviceContext());
	managementCB_->updateCBCamera(managementD3D_->getDeviceContext(),
		vpData.view,
		vpData.viewInv,
		vpData.proj,
		vpData.projInv,
		vpData.eyePos,
		vpData.viewportTopX,
		vpData.viewportTopY,
		vpData.zNear,
		vpData.zFar,
		vpData.viewportWidth,
		vpData.viewportHeight);

	std::map<unsigned int, InstancedData*> instancesMap = managementInstance_->getInstancesMap();
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap.begin(); i != instancesMap.end(); i++)
	{
		renderInstance(i->first, i->second, false);
	}

	//Make me use iterators!
	AttributePtr<Attribute_DebugShape> ptr_debugShape;
	while(itrDebugShape.hasNext())
	{
		ptr_debugShape = itrDebugShape.getNext();
		if(ptr_debugShape->render)
		{
			renderDebugShape(
				ptr_debugShape,
				itrDebugShape.storageIndex(),
				vpData.view, 
				vpData.proj);
		}
	}

	if(settings->showDebugPhysics)
	{
		drawBulletPhysicsDebugLines(vpData.view, vpData.proj);
	}
	
	//Unset and clean.
	managementFX_->unsetAll(devcon);
	managementFX_->unsetLayout(devcon);
	managementSS_->unsetSS(devcon, TypeFX_PS, 0);
	devcon->RSSetState(nullptr);
}
void Renderer::renderViewportToBackBuffer(ViewportData& vpData)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Set backbuffer.
	managementD3D_->setDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);
	managementD3D_->setUAVBackBufferCS();

	//Set shader.
	managementFX_->setShader(devcon, SHADERID_CS_LIGHTING);

	//Set constant buffers.
	managementCB_->setCB(CB_TYPE_FRAME,		TypeFX_CS, CB_REGISTER_FRAME,	devcon);
	managementCB_->setCB(CB_TYPE_CAMERA,	TypeFX_CS, CB_REGISTER_CAMERA,	devcon);
	managementCB_->updateCBCamera(managementD3D_->getDeviceContext(),
		vpData.view,
		vpData.viewInv,
		vpData.proj,
		vpData.projInv,
		vpData.eyePos,
		vpData.viewportTopX,
		vpData.viewportTopY,
		vpData.zNear,
		vpData.zFar,
		vpData.viewportWidth,
		vpData.viewportHeight);

	//Connect g-buffers to shader.
	managementBuffer_->setBuffersAsCSShaderResources(devcon);

	//Set lights.
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_DIR,		LIGHT_SRV_REGISTER_DIR);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_POINT,		LIGHT_SRV_REGISTER_POINT);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_SPOT,		LIGHT_SRV_REGISTER_SPOT);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_POS_VIEW,	LIGHT_SRV_REGISTER_POS);
	
	//Set default samplerstate.
	managementSS_->setSS(devcon, TypeFX_CS, 0, SS_ID_DEFAULT);
	managementSS_->setSS(devcon, TypeFX_CS, 1, SS_ID_SHADOW);

	//Call compute shader kernel.
	unsigned int dispatchX = ceil((float)winfo_->getCSDispathX() / (float)managementViewport_->getNumViewportsX());
	unsigned int dispatchY = ceil((float)winfo_->getCSDispathY() / (float)managementViewport_->getNumViewportsY());
	devcon->Dispatch(dispatchX, dispatchY, 1);

	//Unset and clean.
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_DIR,		LIGHT_SRV_REGISTER_DIR);
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_POINT,	LIGHT_SRV_REGISTER_POINT);
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_SPOT,		LIGHT_SRV_REGISTER_SPOT);
	managementLight_->unsetLightSRVCS(devcon, LIGHTBUFFERTYPE_POS_VIEW,	LIGHT_SRV_REGISTER_POS);

	managementFX_->unsetShader(devcon, SHADERID_CS_LIGHTING);

	managementD3D_->unsetUAVBackBufferCS();
	managementD3D_->unsetDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);
	managementBuffer_->unsetBuffersAsCSShaderResources(devcon);

	managementSS_->unsetSS(devcon, TypeFX_CS, 0);
	managementSS_->unsetSS(devcon, TypeFX_CS, 1);
}

void Renderer::renderInstance(unsigned int meshID, InstancedData* instance, bool shadowmap)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Fetch renderer representation of model.
	ModelD3D* modelD3D	= managementModel_->getModelD3D(meshID, device);

	ShadingDesc shadingDesc = deriveShadingDesc(modelD3D->getVertexType(), shadowmap);
	setShadingDesc(shadingDesc);

	//Set vertex buffer.
	UINT offset[2] = { 0, 0 };
	ID3D11Buffer* vbs[2] = 
	{ 
		modelD3D->getVertexBuffer(), 
		instance->getDataBuffer()
	};
	devcon->IASetVertexBuffers(0, 2, vbs, shadingDesc.stride_, offset);
	
	std::vector<SubsetD3D*>		subsetD3Ds	= modelD3D->getSubsetD3Ds();
	std::vector<MaterialDesc>	materials	= modelD3D->getMaterials();
	for(unsigned int i = 0; i < subsetD3Ds.size(); i++)
	{
		unsigned int materialIndex	= subsetD3Ds[i]->getMaterialIndex();

		renderSubset(
			subsetD3Ds[i],
			materials[materialIndex],
			instance->getDataCountCur(),
			shadowmap);
	}

	//Unset vertex buffers. (In case of the instance buffer needing to be mapped to in ManagementInstance)
	ID3D11Buffer* nullVBs[2] = { NULL, NULL };
	devcon->IASetVertexBuffers(
		0, 
		2, 
		nullVBs, 
		offset, 
		offset);
}

ShadingDesc Renderer::deriveShadingDesc(VertexType vertexType, bool shadowmap)
{
	ShadingDesc shadingDesc;
	switch(vertexType)
	{
	case VERTEX_TYPE_POS_NORM_TEX_TAN:
		{
			shadingDesc.vsID_ = SHADERID_VS_POS_NORM_TEX_TAN_INSTANCE;
			shadingDesc.psID_ = SHADERID_PS_NORMALMAP;

			shadingDesc.layoutID_ = LAYOUTID_POS_NORM_TEX_TAN_INSTANCED;

			shadingDesc.stride_[0] = sizeof(VertexPosNormTexTan);
			shadingDesc.stride_[1] = sizeof(VertexInstanced);
			break;
		}
	default:
		{
			shadingDesc.vsID_ = SHADERID_VS_POS_NORM_TEX_INSTANCE;
			shadingDesc.psID_ = SHADERID_PS_DEFAULT;

			shadingDesc.layoutID_ = LAYOUTID_POS_NORM_TEX_INSTANCED;

			shadingDesc.stride_[0] = sizeof(VertexPosNormTex);
			shadingDesc.stride_[1] = sizeof(VertexInstanced);
			break;
		}
	}

	if(shadowmap)
		shadingDesc.psID_ = SHADERID_PS_BUILD_SHADOWMAP_POS_NORM_TEX;

	return shadingDesc;
}
void Renderer::setShadingDesc(ShadingDesc shadingDesc)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Set shaders
	managementFX_->setShader(devcon, shadingDesc.vsID_);
	managementFX_->setShader(devcon, shadingDesc.psID_);

	//Set layout
	managementFX_->setLayout(devcon, shadingDesc.layoutID_);
}

void Renderer::renderSubset(
	SubsetD3D* subset, 
	MaterialDesc& material, 
	unsigned int numInstances,
	bool shadowmap)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Only set such resources if needed. These are not needed during shadow-pass.
	if(!shadowmap)
	{
		ID3D11ShaderResourceView* texAlbedo = managementTex_->getTexSrv(material.idAlbedoTex_);
		ID3D11ShaderResourceView* texNormal = managementTex_->getTexSrv(material.idNormalTex_);
		devcon->PSSetShaderResources(0, 1, &texAlbedo);
		devcon->PSSetShaderResources(1, 1, &texNormal);
	
		//Set per-subset constant buffer.
		managementCB_->setCB(
			CB_TYPE_SUBSET, 
			TypeFX_PS, 
			CB_REGISTER_SUBSET, 
			devcon);
		DirectX::XMFLOAT3 dxSpec(
			material.specularTerm_.x, 
			material.specularTerm_.y, 
			material.specularTerm_.z);
		managementCB_->updateCBSubset(
			devcon,
			dxSpec,
			material.specularPower_);
	}

	//Set index-buffer.
	UINT offset = 0;
	devcon->IASetIndexBuffer(
		subset->getIndexBuffer(), 
		DXGI_FORMAT_R32_UINT, 
		offset);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw subset.
	devcon->DrawIndexedInstanced(
		subset->getNumIndices(),
		numInstances,
		0, 0, 0);
}
void Renderer::renderDebugShape(
	AttributePtr<Attribute_DebugShape>	ptr_debugShape, 
	unsigned int			shapeIndex,
	DirectX::XMFLOAT4X4		viewMatrix, 
	DirectX::XMFLOAT4X4		projectionMatrix)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();
	
	// Get transform matrices.
	AttributePtr<Attribute_Spatial>	ptr_spatial		= ptr_debugShape->ptr_spatial;
	AttributePtr<Attribute_Position> ptr_position = ptr_spatial->ptr_position;
	DirectX::XMFLOAT4X4 worldMatrix			= managementMath_->calculateWorldMatrix(ptr_spatial, ptr_position);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= managementMath_->calculateMatrixInverse(worldMatrix);
	DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	managementFX_->setShader(devcon, SHADERID_VS_COLOR);
	managementFX_->setShader(devcon, SHADERID_PS_COLOR);

	//Update per-object constant buffer.
	managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
	managementCB_->updateCBObject(
		devcon, 
		finalMatrix, 
		worldMatrix, 
		worldMatrixInverse);
	
	//Fetch renderer representation of shape.
	DebugShapeD3D* shapeD3D = managementModel_->getDebugShapeD3D(shapeIndex, device);
	
	//Set vertex buffer.
	ID3D11Buffer* vertexBuffer	= shapeD3D->getVB()->getVB();
	unsigned int numVertices	= shapeD3D->getVB()->getNumVertices();

	UINT stride = sizeof(VertexPosColor);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
		0, 
		1, 
		&vertexBuffer, 
		&stride, 
		&offset);

	//Set input layout
	managementFX_->setLayout(devcon, LAYOUTID_POS_COLOR);

	//Set topology. Where to put this?
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//Draw subset.
	devcon->Draw(numVertices, 0);
}

//Shadows
DirectX::XMFLOAT4X4	Renderer::buildShadowMap()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	//Get shadow transform:
	SceneBounds bounds;
	bounds.center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); //Origo.
	bounds.radius = sqrtf(25.0f * 25.0f); //Radius of scene really ought to be calculated instead of fixed.

	LightDescDir dirLight = LightDescDir(); //default construct
	AttributePtr<Attribute_Light_Dir> ptr_lightDir;
	if(itrLightDir.hasNext())
	{
		ptr_lightDir = itrLightDir.getNext();
		dirLight = ptr_lightDir->lightDir;

		//static float rotationAngle = 0.0f;
		//rotationAngle += 0.0001f * delta;
		//DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(rotationAngle);
		//
		//DirectX::XMFLOAT3 tempDir = DirectX::XMFLOAT3(dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
		//DirectX::XMVECTOR tempDir2 = XMLoadFloat3(&tempDir);
		//tempDir2 = DirectX::XMVector3TransformNormal(tempDir2, R);
		//DirectX::XMStoreFloat3(&tempDir, tempDir2);
		//
		//ptr_lightDir->lightDir.direction = Float3(tempDir.x, tempDir.y, tempDir.z);
	}
	itrLightDir.resetIndex();

	ShadowMatrices shadowMatrices;
	shadowMatrices = constructShadowMatrices(bounds, dirLight.direction);

	//Set viewport to encompass entire map.
	D3D11_VIEWPORT vp = managementBuffer_->getShadowViewport();
	devcon->RSSetViewports(1, &vp);

	managementRS_->setRS(devcon, RS_ID_DEPTH); //Set rasterizer state with depth bias to avoid shadow acne

	managementBuffer_->setBuffer(
		devcon, 
		SET_ID_SHADOW, 
		SET_TYPE_DSV, 
		SET_STAGE_CS, //stage irrelevant
		0); //register irrelevant

	//Update per-viewport constant buffer.
	managementCB_->setCB(CB_TYPE_CAMERA, TypeFX_VS, CB_REGISTER_CAMERA, managementD3D_->getDeviceContext());
	managementCB_->updateCBCamera(
		managementD3D_->getDeviceContext(),
		/*View: */			shadowMatrices.view_,
		/*View Inverse: */	managementMath_->getIdentityMatrix(),
		/*Proj: */			shadowMatrices.proj_,
		/*Proj Inverse: */	managementMath_->getIdentityMatrix(),
		/*EyePos: */		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),	//Irrelevant
		/*ViewportTopX: */	0,										//Irrelevant					
		/*ViewportTopY: */	0,										//Irrelevant
		/*zNear: */			0.0f,									//Irrelevant
		/*zFar: */			0.0f,									//Irrelevant
		/*ViewportWidth: */ 0.0f,									//Irrelevant
		/*ViewportHeight: */ 0.0f);									//Irrelevant

	std::map<unsigned int, InstancedData*> instancesMap = managementInstance_->getInstancesMap();
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap.begin(); i != instancesMap.end(); i++)
	{
		//if(i->first == 7 ) //Check for what models ought to cast shadows?
		renderInstance(i->first, i->second, true);
	}

	//Unset shizzle
	managementBuffer_->unset(devcon, SET_TYPE_DSV, SET_STAGE_CS, 0); //register and stage irrelevant
	managementRS_->unsetRS(devcon);

	return shadowMatrices.shadowMapTransform_;
}
ShadowMatrices Renderer::constructShadowMatrices(SceneBounds bounds, Float3 lightDirection)
{
	//Position of directional light being the back of viewing frustum.
	DirectX::XMVECTOR lightPos = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(
		-2.0f * bounds.radius * lightDirection.x,
		-2.0f * bounds.radius * lightDirection.y,
		-2.0f * bounds.radius * lightDirection.z)));
	DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&bounds.center); //Looking at center of the scene.
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Build view-matrix:
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(
		lightPos, 
		targetPos, 
		up);

	//Transform bounding sphere to light-space:
	DirectX::XMFLOAT3 sphereCenterLS;
	DirectX::XMStoreFloat3(&sphereCenterLS, DirectX::XMVector3TransformCoord(targetPos, V));

	//Establish frustum:
	float l = sphereCenterLS.x - bounds.radius;
	float b = sphereCenterLS.y - bounds.radius;
	float n = sphereCenterLS.z - bounds.radius;
	float r = sphereCenterLS.x + bounds.radius;
	float t = sphereCenterLS.y + bounds.radius;
	float f = sphereCenterLS.z + bounds.radius;

	//Build projection-matrix:
	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicOffCenterLH(
		l, 
		r, 
		b, 
		t, 
		n, 
		f);

	//Transform NDC-space [-1, +1]^2 to texture space [0, 1]^2
	DirectX::XMMATRIX T(
		0.5f,	0.0f,	0.0f,	0.0f,
		0.0f,	-0.5f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,	0.0f,	1.0f);
	DirectX::XMMATRIX S = V * P * T;

	ShadowMatrices shadowMatrices;
	XMStoreFloat4x4(
		&(shadowMatrices.shadowMapTransform_), 
		S);
	XMStoreFloat4x4(
		&(shadowMatrices.view_), 
		V);
	XMStoreFloat4x4(
		&(shadowMatrices.proj_), 
		P);
	return shadowMatrices;
}

//Glow effect
void Renderer::downSampleBlur()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	D3D11_VIEWPORT viewport = managementBuffer_->getDownSampledViewport();
	devcon->RSSetViewports(1, &viewport);

	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	devcon->IASetVertexBuffers(0, 0, nullptr, 0, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	managementFX_->setShader(devcon, SHADERID_VS_SCREENQUAD);
	managementFX_->setShader(devcon, SHADERID_PS_DOWNSAMPLE);
	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_LOW,
		SET_TYPE_RTV,
		SET_STAGE_PS, //stage irrelevant
		0); //register irrelevant
	
	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_HIGH,
		SET_TYPE_SRV,
		SET_STAGE_PS,
		SHADER_REGISTER_DOWNSAMPLE_INPUT);
	devcon->Draw(4, 0); //Draw four arbitrary vertices.

	managementBuffer_->unset(
		devcon,
		SET_TYPE_SRV,
		SET_STAGE_PS,
		SHADER_REGISTER_DOWNSAMPLE_INPUT);
	
	managementBuffer_->unset(
		devcon,
		SET_TYPE_RTV,
		SET_STAGE_CS, //stage irrelevant
		0); //register irrelevant

	managementFX_->unsetAll(devcon);
	managementSS_->unsetSS(devcon, TypeFX_PS, 0);
}
void Renderer::blurHorizontally()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	
	managementFX_->setShader(devcon, SHADERID_CS_BLUR_HORZ);
	
	//Update glow constant buffer:
	managementCB_->setCB(CB_TYPE_BLUR, TypeFX_CS, CB_REGISTER_BLUR, devcon);
	float blurKernel[11] = 
	{
		0.05f, 
		0.05f, 
		0.1f, 
		0.1f, 
		0.1f, 
		0.2f,
		0.1f,
		0.1f,
		0.1f,
		0.05f,
		0.05f
	};
	managementCB_->updateCBBlur(devcon, blurKernel);
	
	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_LOW,
		SET_TYPE_SRV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_INPUT);

	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_LOW_UTIL,
		SET_TYPE_UAV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_OUTPUT);
	
	unsigned int numBlocksX = (unsigned int)ceilf(winfo_->getScreenWidth() / 256.0f);
	devcon->Dispatch(numBlocksX, winfo_->getScreenHeight(), 1);

	//null shit
	managementBuffer_->unset(
		devcon,
		SET_TYPE_UAV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_OUTPUT);
	
	managementBuffer_->unset(
		devcon,
		SET_TYPE_SRV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_INPUT);
	
	managementFX_->unsetShader(devcon, SHADERID_CS_BLUR_HORZ);
}
void Renderer::blurVertically()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	
	managementFX_->setShader(devcon, SHADERID_CS_BLUR_VERT);
	
	managementCB_->setCB(CB_TYPE_BLUR, TypeFX_CS, CB_REGISTER_BLUR, devcon);
	
	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_LOW,
		SET_TYPE_UAV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_OUTPUT);
	
	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_LOW_UTIL,
		SET_TYPE_SRV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_INPUT);
	
	unsigned int numBlocksY = (unsigned int)ceilf(winfo_->getScreenHeight() / 256.0f);
	devcon->Dispatch(winfo_->getScreenWidth(), numBlocksY, 1);
	
	//null shit
	managementBuffer_->unset(
		devcon,
		SET_TYPE_UAV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_OUTPUT);

	managementBuffer_->unset(
		devcon,
		SET_TYPE_SRV,
		SET_STAGE_CS,
		SHADER_REGISTER_BLUR_INPUT);
	
	managementFX_->unsetAll(devcon); //unsetShader(devcon, SHADERID_CS_BLUR_VERT);
}
void Renderer::upSampleBlur()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	
	D3D11_VIEWPORT upSampleViewport_;
	ZeroMemory(&upSampleViewport_, sizeof(D3D11_VIEWPORT));
	upSampleViewport_.TopLeftX	= 0;
	upSampleViewport_.TopLeftY	= 0;
	upSampleViewport_.Width		= static_cast<FLOAT>(winfo_->getScreenWidth());
	upSampleViewport_.Height	= static_cast<FLOAT>(winfo_->getScreenHeight());
	upSampleViewport_.MinDepth	= 0;
	upSampleViewport_.MaxDepth	= 1;
	devcon->RSSetViewports(1, &upSampleViewport_);
	
	devcon->IASetVertexBuffers(0, 0, nullptr, 0, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	managementFX_->setShader(devcon, SHADERID_VS_SCREENQUAD);
	managementFX_->setShader(devcon, SHADERID_PS_DOWNSAMPLE);

	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_HIGH,
		SET_TYPE_RTV,
		SET_STAGE_PS, //stage irrelevant
		0); //register irrelevant
	managementBuffer_->setBuffer(
		devcon,
		SET_ID_GLOW_LOW,
		SET_TYPE_SRV,
		SET_STAGE_PS,
		SHADER_REGISTER_DOWNSAMPLE_INPUT);

	devcon->Draw(4, 0); //Draw four arbitrary vertices.
	
	managementBuffer_->unset(
		devcon,
		SET_TYPE_RTV,
		SET_STAGE_PS, //stage irrelevant
		0); //register irrelevant
	
	managementBuffer_->unset(
		devcon,
		SET_TYPE_SRV,
		SET_STAGE_PS,
		SHADER_REGISTER_DOWNSAMPLE_INPUT);
	
	managementFX_->unsetAll(devcon);

	managementSS_->unsetSS(devcon, TypeFX_PS, 0);
}

//SSAO
void Renderer::buildOffsetKernel()
{
	//Generate evenly distributed points in a hemisphere oriented along the z-axis:
	for(unsigned int i = 0; i < 14; i++)
	{
		offsetKernel_[i] = DirectX::XMFLOAT4(
			GET_RANDOM(-1.0f, 1.0f),
			GET_RANDOM(-1.0f, 1.0f),
			GET_RANDOM(0.0f, 1.0f),
			0.0f);
		
		//Normalize.
		DirectX::XMVECTOR v = DirectX::XMVector4Normalize(XMLoadFloat4(&offsetKernel_[i]));
		DirectX::XMStoreFloat4(&offsetKernel_[i], v);

		//At this point we have created a hemisphere with sample points distributed along the surface.
		//We then wish to distribute these points in the sphere. First, randomize the lengths of these vectors:
		float scale = GET_RANDOM(0.0f, 1.0f);
		offsetKernel_[i].x *= scale;
		offsetKernel_[i].y *= scale;
		offsetKernel_[i].z *= scale;
	
		//...then create a falloff so that occlussion is less influenced by points further away:
		scale = (float)i / (float)14;
		scale = LERP(0.1f, 1.0f, scale * scale);
		
		offsetKernel_[i].x *= scale;
		offsetKernel_[i].y *= scale;
		offsetKernel_[i].z *= scale;
	}
}
void Renderer::buildSSAOMap(ViewportData& vpData)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	
	managementFX_->setShader(devcon, SHADERID_CS_SSAO);
	
	Buffer_SrvRtvUav* ssaoMap = managementBuffer_->getSSAO();
	
	//Set uav
	ID3D11UnorderedAccessView* uav = ssaoMap->getUAV();
	devcon->CSSetUnorderedAccessViews(
		1, //register 1 
		1, 
		&uav, 
		nullptr);

	//Set normalbuffer as srv
	managementBuffer_->setBuffer(
		devcon, 
		SET_ID_NORMAL, 
		SET_TYPE_SRV, 
		SET_STAGE_CS, 
		0);

	//Set depth as srv
	managementD3D_->setDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);

	//Set random buf
	managementBuffer_->setRandomBuf(devcon, 11);

	managementSS_->setSS(devcon, TypeFX_CS, 0, SS_ID_NORMAL);
	managementSS_->setSS(devcon, TypeFX_CS, 1, SS_ID_DEPTH);
	managementSS_->setSS(devcon, TypeFX_CS, 2, SS_ID_RANDOM);
	
	managementCB_->setCB(CB_TYPE_CAMERA, TypeFX_CS, CB_REGISTER_CAMERA,	devcon);

	unsigned int viewportTopX	= vpData.viewportTopX	/ SSAO_MAP_SCREEN_RES_FACTOR;
	unsigned int viewportTopY	= vpData.viewportTopY	/ SSAO_MAP_SCREEN_RES_FACTOR;
	unsigned int viewportWidth	= vpData.viewportWidth	/ SSAO_MAP_SCREEN_RES_FACTOR;
	unsigned int viewportHeight	= vpData.viewportHeight	/ SSAO_MAP_SCREEN_RES_FACTOR;
	managementCB_->updateCBCamera(managementD3D_->getDeviceContext(),
		vpData.view,
		managementMath_->getIdentityMatrix(),					//Irrelevant
		vpData.proj,
		vpData.projInv,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),					//Irrelevant
		viewportTopX,											//New viewport-dimensions
		viewportTopY,											//New viewport-dimensions
		vpData.viewportWidth,									//Instead used to send original viewport-dimensions.
		vpData.viewportHeight,									//Instead used to send original viewport-dimensions.
		viewportWidth,
		viewportHeight);
	
	managementCB_->setCB(CB_TYPE_SSAO, TypeFX_CS, CB_REGISTER_SSAO, devcon);
	float ssaoWidth		= (float)winfo_->getScreenWidth()	/ (float)SSAO_MAP_SCREEN_RES_FACTOR;
	float ssaoHeight	= (float)winfo_->getScreenHeight()	/ (float)SSAO_MAP_SCREEN_RES_FACTOR;

	managementCB_->updateCBSSAO(
		devcon,
		/*Offset Kernel*/			offsetKernel_,
		/*SSAOMap Width*/			ssaoWidth,
		/*SSAOMap Height*/			ssaoHeight,
		/*Occlusion Radius*/		2.0f);
	
	//Dispatch motherfucker
	unsigned int SSAO_BLOCK_DIM = 16;
	float csDispatchX = ssaoWidth	/ (float)SSAO_BLOCK_DIM;
	float csDispatchY = ssaoHeight	/ (float)SSAO_BLOCK_DIM;
	unsigned int dispatchX = ceil(csDispatchX / (float)managementViewport_->getNumViewportsX());
	unsigned int dispatchY = ceil(csDispatchY / (float)managementViewport_->getNumViewportsY());
	devcon->Dispatch(dispatchX, dispatchY, 1);
	
	//Unser shader
	managementFX_->unsetShader(devcon, SHADERID_CS_SSAO);
	
	//Unset uav
	ID3D11UnorderedAccessView* uavs[] = { nullptr };
	devcon->CSSetUnorderedAccessViews(
		1, //register 1 
		1, 
		uavs, 
		nullptr);

	managementSS_->unsetSS(devcon, TypeFX_CS, 0);
	managementSS_->unsetSS(devcon, TypeFX_CS, 1);
	managementSS_->unsetSS(devcon, TypeFX_CS, 2);
	
	managementBuffer_->unset(devcon, SET_TYPE_SRV, SET_STAGE_CS, 0);
	managementD3D_->unsetDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);
	managementBuffer_->unset(devcon, SET_TYPE_SRV, SET_STAGE_CS, 11);
}

void Renderer::drawBulletPhysicsDebugLines(
	DirectX::XMFLOAT4X4		viewMatrix, 
	DirectX::XMFLOAT4X4		projectionMatrix)
{
	//Bullet Physics debug lines are stored as a pointer to an std::vector containing "VertexPosColor" in an event (refer to debugDrawDispatcher::drawLine).
	//The event is fetched and handled from here. Afterwards the event queue is flushed using FLUSH_QUEUED_EVENTS.
	static unsigned int nrOfDebugLines = 0;

	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Update (recreate) vertex buffer
	std::vector<Event*>* debugDrawEvents = GET_POINTER_TO_QUEUED_EVENTS(EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES);
	if(debugDrawEvents->size() > 0)
	{
		SAFE_RELEASE(debugLinesVertexBuffer_);
		
		//Assume that debugDrawEvents contains one event, i.e. handle only one event. All events, including this one, should be flushed correctly below using FLUSH_QUEUED_EVENTS.
		Event* e = debugDrawEvents->at(0);
		EventType type = e->getType();
		if(type == EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES)
		{
			Event_DrawBulletPhysicsDebugLines* debugDraw = static_cast<Event_DrawBulletPhysicsDebugLines*>(e);
			std::vector<VertexPosColor>* debugLines = debugDraw->debugLineVertices; //Vertex vector extracted from event
			nrOfDebugLines = debugDraw->debugLineVertices->size(); //Vertex vector extracted from event

			D3D11_BUFFER_DESC vbd;
			vbd.Usage			= D3D11_USAGE_DYNAMIC;
			vbd.ByteWidth		= sizeof(VertexPosColor) * nrOfDebugLines;
			vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			vbd.MiscFlags		= 0;

			D3D11_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = debugLines->data();

			//Create vertex buffer
			HRESULT hr;
			hr = device->CreateBuffer(&vbd, &vinitData, &debugLinesVertexBuffer_);
		}
	}
	FLUSH_QUEUED_EVENTS(EVENT_DRAW_BULLET_PHYSICS_DEBUG_LINES);

	//Draw Bullet Physics debug lines
	if(debugLinesVertexBuffer_)
	{
		DirectX::XMFLOAT4X4 identityMatrix
		(
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		);

		DirectX::XMFLOAT4X4 worldMatrix			= identityMatrix;
		DirectX::XMFLOAT4X4 worldMatrixInverse	= identityMatrix;
		DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
					
		managementFX_->setShader(devcon, SHADERID_VS_COLOR);
		managementFX_->setShader(devcon, SHADERID_PS_COLOR);

		//Update per-object constant buffer.
		managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
		managementCB_->updateCBObject(
			devcon, 
			finalMatrix, 
			worldMatrix, 
			worldMatrixInverse);

		UINT stride = sizeof(VertexPosColor);
		UINT offset = 0;
		devcon->IASetVertexBuffers(
			0, 
			1, 
			&debugLinesVertexBuffer_, 
			&stride, 
			&offset);

		managementFX_->setLayout(devcon, LAYOUTID_POS_COLOR);

		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		devcon->Draw(nrOfDebugLines, 0);
	}
}

void Renderer::renderHudElements(int viewportIndex)
{
	//Since drawn directly in screen space hud elements are not affected by the camera's aspect ratio,
	//Therefor when there are two viewports the hud elements needs to be scaled by 0.5 along the x-axis
	//to keep thier proportions.
	float scaleModifierX = 1.0f;
	int numViewports = managementViewport_->getNumViewportsX() * managementViewport_->getNumViewportsY();
	if(numViewports == 2)
		scaleModifierX = 0.5f;

	renderHudElementCrossHair(viewportIndex, scaleModifierX);
}
void Renderer::renderHudElementCrossHair(int viewportIndex, float scaleModifierX)
{
	DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(0.0f);
	DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(0.0f);
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(0.2f * scaleModifierX, 0.2f, 1.0f);

	DirectX::XMMATRIX spriteMatrix = rotationX * rotationY * rotationZ * scaling * translation;
	DirectX::XMFLOAT4X4 transformationMatrix;
	DirectX::XMStoreFloat4x4(&transformationMatrix, spriteMatrix);

	drawHudElement(viewportIndex, TEXTURE_ID_CROSS_HAIR, transformationMatrix);
}
void Renderer::drawHudElement(int viewportIndex, unsigned int textureId, DirectX::XMFLOAT4X4 transformationMatrix)
{
	ID3D11Device*		 device = managementD3D_->getDevice();
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	managementCB_->updateCBSprite(devcon, transformationMatrix);
	managementCB_->setCB(CB_TYPE_SPRITE, TypeFX_VS, CB_REGISTER_SPRITE, devcon);

	managementViewport_->setViewport(devcon, viewportIndex);

	managementFX_->setShader(devcon, SHADERID_VS_SPRITE);
	managementFX_->setShader(devcon, SHADERID_PS_SPRITE);
	
	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	ID3D11ShaderResourceView* tex = managementTex_->getTexSrv(textureId);
	devcon->PSSetShaderResources(0, 1, &tex);

	managementD3D_->setRenderTargetViewBackBuffer();
	//6managementD3D_->setUAVBackBufferCS();

	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX);
	
	ID3D11Buffer* vertexBuffer = managementSprites_->getVertexBuffer();
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(
				0, 
				1, 
				&vertexBuffer, 
				&stride, 
				&offset);
	devcon->IASetIndexBuffer(managementSprites_->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(NUM_SPRITE_INDICES, 0, 0);

	managementD3D_->unsetRenderTargetViewBackBuffer();
	//managementD3D_->unsetUAVBackBufferCS();

	managementFX_->unsetAll(devcon);
	managementSS_->unsetSS(devcon, TypeFX_PS, 0);
	//devcon->PSSetSamplers(0, 0, nullptr);
	devcon->IASetInputLayout(nullptr);
	devcon->RSSetState(nullptr);
}

void Renderer::renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
//	ID3D11Device*			device = managementD3D_->getDevice();
//	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();
//
//	DirectX::XMFLOAT4X4 worldMatrix(0.01f, 0.0f, 0.0f, 0.0f,
//									0.0f, 0.01f, 0.0f, 0.0f,
//									0.0f, 0.0f, 0.01f, 0.0f,
//									10.0f, 2.3f, 1.0f, 1.0f);
//	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
//	DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
//	
//	managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
//	managementCB_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
//	
//	animatedMesh_->update(0.002f);
//	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
//	animatedMesh_->getSkinInfo()->getFinalTransforms("Take1", animatedMesh_->getTimePosition(), &finalTransforms);
//
//	managementCB_->setCB(CB_TYPE_BONE, TypeFX_VS, CB_REGISTER_BONE, devcon);
//	managementCB_->updateCBBone(devcon, finalTransforms);
//
//	managementFX_->setShader(devcon, SHADERID_VS_ANIMATION);
//	managementFX_->setShader(devcon, SHADERID_PS_ANIMATION);
//
//	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
//	managementRS_->setRS(devcon, RS_ID_DEFAULT);
//
//	managementGBuffer_->setGBuffersAndDepthBuffer(devcon, managementD3D_->getDepthBuffer());
//
//	managementD3D_->clearDepthBuffer();
//
//	ID3D11Buffer* vertexBuffer = animatedMesh_->getVertexBuffer();
//	UINT stride = sizeof(VertexPosNormTexTanSkinned);
//	UINT offset = 0;
//	devcon->IASetVertexBuffers(
//				0, 
//				1, 
//				&vertexBuffer, 
//				&stride, 
//				&offset);
//	devcon->IASetIndexBuffer(animatedMesh_->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
//	
//	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX_TAN_SKINNED);
//
//	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	devcon->DrawIndexed(animatedMesh_->getNumIndices(), 0, 0);
//
//	managementGBuffer_->unsetGBuffersAndDepthBufferAsRenderTargets(devcon);
//
//	managementFX_->unsetAll(devcon);
//
//	devcon->PSSetSamplers(0, 0, nullptr);
//	devcon->IASetInputLayout(nullptr);
//	devcon->RSSetState(nullptr);
}

void Renderer::loadTextures(TexDesc* texdesc)
{
	managementTex_->handleTexDesc(texdesc, managementD3D_->getDevice());
}