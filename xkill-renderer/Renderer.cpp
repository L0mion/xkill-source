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
#include "ManagementMath.h"
#include "ManagementInstance.h"

#include "Winfo.h"
#include "ModelD3D.h"
#include "gBuffer.h"
#include "SubsetD3D.h"
#include "DebugShapeD3D.h"
#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "TypeFX.h"
#include "Renderer.h"
#include "ViewportData.h"

//temp
#include "AnimatedMesh.h"
#include "M3DLoader.h"
#include "SkinnedData.h"

ATTRIBUTES_DECLARE_ALL;

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
	managementMath_		= nullptr;
	managementInstance_ = nullptr;

	attributesRenderOwner_	= nullptr;

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
	SAFE_DELETE(managementGBuffer_);
	SAFE_DELETE(managementMath_);
	SAFE_DELETE(managementInstance_);

	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(managementDebug_);

	SAFE_RELEASE(debugLinesVertexBuffer_);

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
	SAFE_RESET(managementSS_);
	SAFE_RESET(managementRS_);
	SAFE_RESET(managementGBuffer_);
}
HRESULT Renderer::resize(unsigned int screenWidth, unsigned int screenHeight)
{
	//Get number of split-screens
	unsigned int numSS = itrSplitScreen.size();
	
	//Initialize new windo-type object.
	HRESULT hr = S_OK;
	unsigned int numViewports, csDispatchX, csDispatchY;
	numViewports	= numSS;
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
	initManagementInstance();

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
	numViewports	= itrCamera.size();
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
void Renderer::initManagementMath()
{
	managementMath_ = new ManagementMath();
}
void Renderer::initManagementInstance()
{
	managementInstance_ = new ManagementInstance();
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
	managementGBuffer_->clearGBuffers(devcon);
	managementD3D_->clearDepthBuffer();

	//Update per-frame constant buffer.
	managementCB_->setCB(
		CB_TYPE_FRAME, 
		TypeFX_VS, 
		CB_REGISTER_FRAME, 
		devcon);
	managementCB_->updateCBFrame(
		devcon,
		managementLight_->getLightDirCurCount(),
		managementLight_->getLightPointCurCount(),
		managementLight_->getLightSpotCurCount());

	Attribute_Camera*		camAt; 
	Attribute_Spatial*		spatialAt;
	Attribute_Position*		posAt;
	Attribute_SplitScreen*	ssAt;

	ViewportData vpData;

	//Render each split-screen seperately
	std::vector<SplitScreenViewport>* ssViewports = managementViewport_->getSplitScreenViewports();
	std::vector<ViewportData> vpDatas(ssViewports->size());
	for(unsigned int i = 0; i < ssViewports->size(); i++)
	{
		ssAt		= ssViewports->at(i).ssAt;
		camAt		= itrCamera.at(ssAt->ptr_camera);

		spatialAt	= ATTRIBUTE_CAST(Attribute_Spatial, ptr_spatial, camAt);
		posAt		= ATTRIBUTE_CAST(Attribute_Position, ptr_position, spatialAt);

		managementViewport_->setViewport(devcon, i);

		//Store all the viewport-specific data for the backbuffer-rendering.
		vpData.camIndex		= ssAt->ptr_camera.index;
		vpData.view			= DirectX::XMFLOAT4X4(((float*)&camAt->mat_view));
		vpData.proj			= DirectX::XMFLOAT4X4(((float*)&camAt->mat_projection));
		vpData.viewInv		= managementMath_->calculateMatrixInverse(vpData.view);
		vpData.projInv		= managementMath_->calculateMatrixInverse(vpData.proj);
		vpData.eyePos		= *(DirectX::XMFLOAT3*)&posAt->position;
		vpData.viewportTopX = static_cast<unsigned int>(ssAt->ssTopLeftX);
		vpData.viewportTopY = static_cast<unsigned int>(ssAt->ssTopLeftY);
		vpData.zNear		= camAt->zNear;
		vpData.zFar			= camAt->zFar;
		vpDatas[i]			= vpData;

		renderViewportToGBuffer(vpData);
	}

	//Render everything to backbuffer.
	for(unsigned int i = 0; i < vpDatas.size(); i++)
		renderViewportToBackBuffer(vpDatas[i]);

	managementD3D_->present();
}
void Renderer::renderViewportToGBuffer(ViewportData& vpData)									
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	if(animatedMesh_)
		renderAnimatedMesh(vpData.view, vpData.proj);

	managementFX_->setShader(devcon, SHADERID_VS_DEFAULT);
	managementFX_->setShader(devcon, SHADERID_PS_DEFAULT);

	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBuffer(devcon, managementD3D_->getDepthBuffer());

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
		vpData.zFar);

	std::map<unsigned int, InstancedData*> instancesMap = managementInstance_->getInstancesMap();
	for(std::map<unsigned int, InstancedData*>::iterator i = instancesMap.begin(); i != instancesMap.end(); i++)
	{
		renderInstance(i->first, i->second);
	}

	//Render renderattributes
	//Attribute_Render* renderAt;
	//while(itrRender.hasNext())
	//{
	//	renderAt = itrRender.getNext();
	//	renderAttribute(
	//		renderAt, 
	//		vpData.view, 
	//		vpData.proj);
	//}

	//Make me use iterators!
	Attribute_DebugShape* debugShapeAt;
	while(itrDebugShape.hasNext())
	{
		debugShapeAt = itrDebugShape.getNext();
		if(debugShapeAt->render)
		{
			renderDebugShape(
				debugShapeAt,
				itrDebugShape.storageIndex(),
				vpData.view, 
				vpData.proj);
		}
	}

	//if(BULLETPHYSICSDEBUGDRAW)
	//{
	//	drawBulletPhysicsDebugLines(vpData.view, vpData.proj);
	//}

	
	managementGBuffer_->unsetGBuffersAndDepthBufferAsRenderTargets(devcon);

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
	managementFX_->setShader(devcon, SHADERID_CS_DEFAULT);

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
		vpData.zFar);

	//Connect g-buffers to shader.
	managementGBuffer_->setGBuffersAsCSShaderResources(devcon);

	//Set lights.
	managementLight_->transformLightViewSpacePoss(devcon, vpData.view);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_DIR,		LIGHT_SRV_REGISTER_DIR);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_POINT,		LIGHT_SRV_REGISTER_POINT);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_SPOT,		LIGHT_SRV_REGISTER_SPOT);
	managementLight_->setLightSRVCS(devcon, LIGHTBUFFERTYPE_POS_VIEW,	LIGHT_SRV_REGISTER_POS);
	
	//Set default samplerstate.
	managementSS_->setSS(devcon, TypeFX_CS, 0, SS_ID_DEFAULT);

	//Call compute shader kernel.
	unsigned int dispatchX = winfo_->getCSDispathX() / managementViewport_->getNumViewportsX();
	unsigned int dispatchY = winfo_->getCSDispathY() / managementViewport_->getNumViewportsY();
	devcon->Dispatch(dispatchX, dispatchY, 1);

	//Unset and clean.
	managementFX_->unsetShader(devcon, SHADERID_CS_DEFAULT);

	managementD3D_->unsetUAVBackBufferCS();
	managementD3D_->unsetDepthBufferSRV(GBUFFER_SHADER_REGISTER_DEPTH);
	managementGBuffer_->unsetGBuffersAsCSShaderResources(devcon);

	devcon->CSSetSamplers(0, 0, nullptr); //move me into managementSS
}

void Renderer::renderInstance(unsigned int meshID, InstancedData* instance)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Fetch renderer representation of model.
	ModelD3D* modelD3D	= managementModel_->getModelD3D(meshID, device);

	//Set vertex buffer.
	UINT stride[2] = { sizeof(VertexPosNormTex), sizeof(VertexPosNormTexInstanced) };
	UINT offset[2] = { 0, 0 };
	ID3D11Buffer* vbs[2] = 
	{ 
		modelD3D->getVertexBuffer()->getVB(), 
		instance->getInstanceBuffer()
	};
	devcon->IASetVertexBuffers(0, 2, vbs, stride, offset);
	
	std::vector<SubsetD3D*>		subsetD3Ds	= modelD3D->getSubsetD3Ds();
	std::vector<MeshMaterial>	materials	= modelD3D->getMaterials();
	for(unsigned int i = 0; i < subsetD3Ds.size(); i++)
	{
		IB* ib	= subsetD3Ds[i]->getIndexBuffer();
		unsigned int materialIndex	= subsetD3Ds[i]->getMaterialIndex();

		renderSubset(
			ib,
			materials[materialIndex],
			instance->getInstanceCount());
	}
}
void Renderer::renderSubset(
	IB* ib, 
	MeshMaterial& material, 
	unsigned int numInstances)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	//Set textures.
	ID3D11ShaderResourceView* texAlbedo = managementTex_->getTexSrv(material.getIDAlbedoTex());
	ID3D11ShaderResourceView* texNormal = managementTex_->getTexSrv(material.getIDNormalTex());
	devcon->PSSetShaderResources(0, 1, &texAlbedo);
	devcon->PSSetShaderResources(1, 1, &texNormal);

	//Set per-subset constant buffer.
	managementCB_->setCB(CB_TYPE_SUBSET, TypeFX_PS, CB_REGISTER_SUBSET, devcon);
	DirectX::XMFLOAT3 dxSpec(1.0f, 1.0f, 1.0f); //((float*)&material.getSpecularTerm());
	managementCB_->updateCBSubset(
		devcon,
		dxSpec,
		material.getSpecularPower());

	//Set input layout
	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX_INSTANCED);

	//Set index-buffer.
	UINT offset = 0;
	devcon->IASetIndexBuffer(
		ib->getIB(), 
		DXGI_FORMAT_R32_UINT, 
		offset);

	//Set topology. Where to put this?
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw subset.
	devcon->DrawIndexedInstanced(
		ib->getNumIndices(),
		numInstances,
		0, 0, 0);
}
void Renderer::renderDebugShape(
	Attribute_DebugShape*	debugShapeAt, 
	unsigned int			shapeIndex,
	DirectX::XMFLOAT4X4		viewMatrix, 
	DirectX::XMFLOAT4X4		projectionMatrix)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();
	
	//Get transform matrices.
	Attribute_Spatial*	spatialAt			= itrSpatial.at(debugShapeAt->ptr_spatial.index);
	Attribute_Position*	positionAt			= itrPosition.at(spatialAt->ptr_position.index);
	DirectX::XMFLOAT4X4 worldMatrix			= managementMath_->calculateWorldMatrix(spatialAt, positionAt);
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

void Renderer::renderAnimatedMesh(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	ID3D11Device*			device = managementD3D_->getDevice();
	ID3D11DeviceContext*	devcon = managementD3D_->getDeviceContext();

	DirectX::XMFLOAT4X4 worldMatrix(0.01f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.01f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.01f, 0.0f,
									10.0f, 2.3f, 1.0f, 1.0f);
	DirectX::XMFLOAT4X4 worldMatrixInverse	= worldMatrix;
	DirectX::XMFLOAT4X4 finalMatrix			= managementMath_->calculateFinalMatrix(worldMatrix, viewMatrix, projectionMatrix);
	
	managementCB_->setCB(CB_TYPE_OBJECT, TypeFX_VS, CB_REGISTER_OBJECT, devcon);
	managementCB_->updateCBObject(devcon, finalMatrix, worldMatrix, worldMatrixInverse);
	
	animatedMesh_->update(0.002f);
	std::vector<DirectX::XMFLOAT4X4> finalTransforms;
	animatedMesh_->getSkinInfo()->getFinalTransforms("Take1", animatedMesh_->getTimePosition(), &finalTransforms);

	managementCB_->setCB(CB_TYPE_BONE, TypeFX_VS, CB_REGISTER_BONE, devcon);
	managementCB_->updateCBBone(devcon, finalTransforms);

	managementFX_->setShader(devcon, SHADERID_VS_ANIMATION);
	managementFX_->setShader(devcon, SHADERID_PS_ANIMATION);

	managementSS_->setSS(devcon, TypeFX_PS, 0, SS_ID_DEFAULT);
	managementRS_->setRS(devcon, RS_ID_DEFAULT);

	managementGBuffer_->setGBuffersAndDepthBuffer(devcon, managementD3D_->getDepthBuffer());

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
	
	managementFX_->setLayout(devcon, LAYOUTID_POS_NORM_TEX_TAN_SKINNED);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(animatedMesh_->getNumIndices(), 0, 0);

	managementGBuffer_->unsetGBuffersAndDepthBufferAsRenderTargets(devcon);

	managementFX_->unsetAll(devcon);

	devcon->PSSetSamplers(0, 0, nullptr);
	devcon->IASetInputLayout(nullptr);
	devcon->RSSetState(nullptr);
}

void Renderer::loadTextures(TexDesc* texdesc)
{
	managementTex_->handleTexDesc(texdesc, managementD3D_->getDevice());
}