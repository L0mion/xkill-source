#include "renderingComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <DirectXMath.h>

#include "D3DManagement.h"
#include "fxManagement.h"
#include "ViewportManagement.h"
#include "gBuffer.h"
#include "renderingUtilities.h"
#include "d3dDebug.h"
#include "CBManagement.h"
#include "LightManagement.h"
#include "objLoaderBasic.h"
#include "mathBasic.h"
#include "vertices.h"

#include <xkill-utilities/EventManager.h>

RenderingComponent::RenderingComponent(HWND windowHandle)
{
	GET_ATTRIBUTES(renderAttributes_, RenderAttribute, ATTRIBUTE_RENDER);
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
	viewportManagement_ = nullptr;
	d3dDebug_	= nullptr;
	
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;
	
	//temp
	vertexBuffer_	= nullptr;
	vertices_		= nullptr;
	objLoader_		= nullptr;
}
RenderingComponent::~RenderingComponent()
{
	SAFE_DELETE(d3dManagement_);
	SAFE_DELETE(fxManagement_);
	SAFE_DELETE(cbManagement_);
	SAFE_DELETE(lightManagement_);
	SAFE_DELETE(viewportManagement_);
	
	//d3dDebug_->reportLiveDeviceObjects();
	SAFE_DELETE(d3dDebug_);

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);

	//temp
	SAFE_RELEASE(vertexBuffer_);
	SAFE_DELETE(objLoader_);
}
HRESULT RenderingComponent::init()
{
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
		hr = initViewport();
//	if(SUCCEEDED(hr))
//		hr = initDebug();
	if(SUCCEEDED(hr))
		hr = initGBuffers();
	
	//temp
	if(SUCCEEDED(hr)) 
		hr = initVertexBuffer();

	return hr;
}
void RenderingComponent::reset()
{
	if(d3dManagement_)
		d3dManagement_->reset();
	if(fxManagement_)
		fxManagement_->reset();
	if(viewportManagement_)
		viewportManagement_->reset();
	if(cbManagement_)
		cbManagement_->reset();
	if(lightManagement_)
		lightManagement_->reset();

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		if(gBuffers_[i])
			gBuffers_[i]->reset();
	

	//temp
	SAFE_RELEASE(vertexBuffer_);

	EventManager::getInstance();
}

void RenderingComponent::onUpdate(float delta)
{
	clearGBuffers();
	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		DirectX::XMFLOAT4X4 view((float*)&cameraAttributes_->at(i).mat_view);
		DirectX::XMFLOAT4X4 projection((float*)&cameraAttributes_->at(i).mat_projection);

		DirectX::XMFLOAT4X4 viewInverse			= calculateMatrixInverse(view);
		DirectX::XMFLOAT4X4 projectionInverse	= calculateMatrixInverse(projection);
		
		SpatialAttribute*	spatialAttribute = static_cast<SpatialAttribute*>(cameraAttributes_->at(i).spatialAttribute.host);
		PositionAttribute*	positionAttribute = static_cast<PositionAttribute*>(spatialAttribute->positionAttribute.host);
		DirectX::XMFLOAT3	eyePosition = *(DirectX::XMFLOAT3*)&positionAttribute->position;
		//DirectX::XMFLOAT3 eyePosition(0.0f, 0.0f, -50.0f);

		setViewport(i);
		renderToGBuffer(view, viewInverse, projection, projectionInverse, eyePosition);
	}
	
	renderToBackBuffer();
}

void RenderingComponent::render(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection)
{
	//renderToGBuffer(view, projection);
	renderToBackBuffer();
}
void RenderingComponent::renderToGBuffer(DirectX::XMFLOAT4X4 view,
										 DirectX::XMFLOAT4X4 viewInverse,
										 DirectX::XMFLOAT4X4 projection,
										 DirectX::XMFLOAT4X4 projectionInverse,
										 DirectX::XMFLOAT3	eyePosition)
{

	d3dManagement_->getDeviceContext()->VSSetShader(fxManagement_->getDefaultVS()->getVertexShader(), nullptr, 0);
	d3dManagement_->getDeviceContext()->PSSetShader(fxManagement_->getDefaultPS()->getPixelShader(), nullptr, 0);
	d3dManagement_->setSSDefaultPS();
	d3dManagement_->setRSDefault();

	gBufferRenderSetRenderTargets();
	
	d3dManagement_->clearDepthBuffer();

	// Fetch attributes
	std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>* allPosition;	GET_ATTRIBUTES(allPosition, PositionAttribute, ATTRIBUTE_POSITION);
	
	for(unsigned int i=0; i<renderAttributes_->size(); i++)
	{
		DirectX::XMFLOAT4X4 finalMatrix = calculateFinalMatrix(view, projection, allSpatial->at(i), allPosition->at(i),  i);
		gBufferRenderUpdateConstantBuffers(finalMatrix, view, viewInverse, projection, projectionInverse, eyePosition);

		UINT stride = sizeof(VertexPosNormTex);
		UINT offset = 0;
		d3dManagement_->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
		d3dManagement_->getDeviceContext()->IASetInputLayout(fxManagement_->getILDefaultVSPosNormTex());
		d3dManagement_->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dManagement_->getDeviceContext()->Draw(vertices_->size(), 0);
	}

	renderClean();
}
void RenderingComponent::renderToBackBuffer()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	d3dManagement_->setUAVBackBufferCS();

	cbManagement_->csSet(CB_FRAME_INDEX, 0, d3dManagement_->getDeviceContext());
	cbManagement_->csSet(CB_INSTANCE_INDEX, 1, d3dManagement_->getDeviceContext());
	cbManagement_->updateCBInstance(d3dManagement_->getDeviceContext(), screenWidth_, screenHeight_);

	lightManagement_->setLightSRVCS(d3dManagement_->getDeviceContext(), 2);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();
	d3dManagement_->getDeviceContext()->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	d3dManagement_->setSSDefaultCS();

	fxManagement_->getDefaultCS()->set(d3dManagement_->getDeviceContext());
	d3dManagement_->getDeviceContext()->Dispatch(25, 25, 1);
	fxManagement_->getDefaultCS()->unset(d3dManagement_->getDeviceContext());

	ID3D11UnorderedAccessView* uav2[] = { nullptr };
	d3dManagement_->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, uav2, NULL);
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = nullptr;
	d3dManagement_->getDeviceContext()->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	d3dManagement_->getDeviceContext()->CSSetSamplers(0, 0, nullptr);

	d3dManagement_->present();

	renderClean();
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

void RenderingComponent::gBufferRenderUpdateConstantBuffers(DirectX::XMFLOAT4X4 finalMatrix,
															DirectX::XMFLOAT4X4 viewMatrix,
															DirectX::XMFLOAT4X4 viewInverseMatrix,
															DirectX::XMFLOAT4X4 projectionMatrix,
															DirectX::XMFLOAT4X4 projectionInverseMatrix,
															DirectX::XMFLOAT3	eyePosition)
{
	cbManagement_->vsSet(CB_FRAME_INDEX, 0, d3dManagement_->getDeviceContext());
	cbManagement_->updateCBFrame(d3dManagement_->getDeviceContext(),
								 finalMatrix,
								 viewMatrix,
								 viewInverseMatrix,
								 projectionMatrix,
								 projectionInverseMatrix,
								 eyePosition, 
								 lightManagement_->getNumLights());
}
void RenderingComponent::renderClean()
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
void RenderingComponent::gBufferRenderSetRenderTargets()
{
	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	d3dManagement_->getDeviceContext()->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, d3dManagement_->getDepthBuffer());
}

DirectX::XMFLOAT4X4 RenderingComponent::calculateFinalMatrix(DirectX::XMFLOAT4X4 viewMatrix,
															 DirectX::XMFLOAT4X4 projectionMatrix,
															 SpatialAttribute spatialAttribute,
															 PositionAttribute positionAttribute,
															 unsigned int attributeIndex)
{
	DirectX::CXMMATRIX mView		= DirectX::XMLoadFloat4x4(&viewMatrix);
	DirectX::CXMMATRIX mProjection	= DirectX::XMLoadFloat4x4(&projectionMatrix);


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
	DirectX::XMMATRIX mFinalMatrix = mWorldMatrix * mView * mProjection;
	
	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, mFinalMatrix);

	return finalMatrix;
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

HRESULT RenderingComponent::initD3DManagement()
{
	HRESULT hr;
	d3dManagement_ = new D3DManagement(windowHandle_, screenWidth_, screenHeight_);
	hr = d3dManagement_->init();
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
HRESULT RenderingComponent::initViewport()
{
	HRESULT hr = S_OK;
	viewportManagement_ = new ViewportManagement(numViewports_,
												 screenWidth_,
												 screenHeight_);
	hr = viewportManagement_->init();

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
HRESULT RenderingComponent::initDebug()
{
	HRESULT hr = S_OK;

	d3dDebug_ = new D3DDebug();
	hr = d3dDebug_->init(d3dManagement_->getDevice());

	return hr;
}
HRESULT RenderingComponent::initVertexBuffer()
{
	HRESULT hr = S_OK;

	vertices_ = new std::vector<VertexPosNormTex>();
	objLoader_ = new ObjLoaderBasic();
	objLoader_->parseObjectFile("../../xkill-resources/xkill-models/bth.obj", vertices_);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexPosNormTex) * vertices_->size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices_->at(0);
	d3dManagement_->getDevice()->CreateBuffer(&vbd, &vinitData, &vertexBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initVertexBuffer CreateBuffer failed");

	return hr;
}

void RenderingComponent::onEvent( Event* e )
{

}
