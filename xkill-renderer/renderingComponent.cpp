#include <renderingComponent.h>
#include <fxManagement.h>
#include <gBuffer.h>
#include <renderingUtilities.h>
#include <d3dDebug.h>
#include <CBManagement.h>
#include <objLoaderBasic.h>

RenderingComponent::RenderingComponent(HWND windowHandle,
									   unsigned int screenWidth,
									   unsigned int screenHeight,
									   unsigned int aliasingCount)
{
	this->windowHandle_	= windowHandle;
	this->screenWidth_	= screenWidth;
	this->screenHeight_	= screenHeight;
	this->aliasingCount_	= aliasingCount;

	fxManagement_	= nullptr;
	cbManagement_	= nullptr; 
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers_[i] = nullptr;
	d3dDebug_		= nullptr;

	device_	= nullptr;
	devcon_	= nullptr;
	swapChain_ = nullptr;

	rtvBackBuffer_	= nullptr;
	uavBackBuffer_	= nullptr;
	dsvDepthBuffer_	= nullptr;
	rsDefault_		= nullptr;
	ssDefault_		= nullptr;
	
	texBackBuffer_	= nullptr;
	texDepthBuffer_	= nullptr;

	//temp
	vertexBuffer_ = nullptr;
	vertices_ = nullptr;
	objLoader_ = nullptr;
}
RenderingComponent::~RenderingComponent()
{
	if(swapChain_)
	{
		swapChain_->SetFullscreenState(false, nullptr);
		SAFE_RELEASE(swapChain_);
	}
	
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);
	SAFE_RELEASE(rsDefault_);
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);

	SAFE_RELEASE(vertexBuffer_); //temp

	SAFE_DELETE(cbManagement_);
	SAFE_DELETE(fxManagement_);
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers_[i]);
	
	//d3dDebug->reportLiveDeviceObjects();
	SAFE_DELETE(d3dDebug_);

	//temp
	SAFE_DELETE(vertices_);
	SAFE_DELETE(objLoader_);
}

HRESULT RenderingComponent::init()
{
	HRESULT hr = S_OK;

	if(SUCCEEDED(hr))
		hr = initDeviceAndSwapChain();
	if(SUCCEEDED(hr))
		hr = initDepthBuffer();
	if(SUCCEEDED(hr))
		hr = initBackBuffer();
	if(SUCCEEDED(hr))
		hr = initGBuffers();
	if(SUCCEEDED(hr))
		initViewport();
	if(SUCCEEDED(hr))
		hr = initRSDefault();
	if(SUCCEEDED(hr))
		hr = initSSDefault();
	if(SUCCEEDED(hr))
		hr = initFXManagement();
	if(SUCCEEDED(hr))
		hr = initCBManagement();
	if(SUCCEEDED(hr)) //temp
		hr = initVertexBuffer();
	if(SUCCEEDED(hr))
		hr = initDebug();

	return hr;
}

void RenderingComponent::reset()
{
	if(swapChain_)
	{
		swapChain_->SetFullscreenState(false, nullptr);
		SAFE_RELEASE(swapChain_);
	}
	
	if(fxManagement_)
		fxManagement_->reset();
	if(cbManagement_)
		cbManagement_->reset();

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		if(gBuffers_[i])
			gBuffers_[i]->reset();
	
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);
	SAFE_RELEASE(rsDefault_);
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);
	//SAFE_RELEASE(cbPerFrame_);

	//temp
	SAFE_RELEASE(vertexBuffer_);
}

void RenderingComponent::render(MatF4 view, MatF4 projection)
{
	renderToGBuffer(view, projection);
	renderToBackBuffer();
}
void RenderingComponent::renderToGBuffer(MatF4 view, MatF4 projection)
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	devcon_->VSSetShader(fxManagement_->getDefaultVS()->getVertexShader(), nullptr, 0);
	devcon_->PSSetShader(fxManagement_->getDefaultPS()->getPixelShader(), nullptr, 0);
	devcon_->PSSetSamplers(0, 1, &ssDefault_);

	cbManagement_->getCBPerFrame()->update(devcon_, view * projection);
	cbManagement_->getCBPerFrame()->vsSet(devcon_);

	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers_[i]->getRTV();
	devcon_->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, dsvDepthBuffer_);
	devcon_->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO], green);
	devcon_->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL], blue);

	devcon_->ClearDepthStencilView(dsvDepthBuffer_, D3D11_CLEAR_DEPTH, 1.0f, 0);
	devcon_->RSSetState(rsDefault_);
	//devcon->IASetVertexBuffers(0, 0, NULL, 0, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	devcon_->IASetInputLayout(fxManagement_->getInputLayout());
	devcon_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon_->Draw(vertices_->size(), 0);

	devcon_->VSSetShader(NULL, NULL, 0);
	devcon_->PSSetShader(NULL, NULL, 0);
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = nullptr;
	devcon_->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, nullptr);
	devcon_->PSSetSamplers(0, 0, nullptr);
	devcon_->IASetInputLayout(nullptr);
	devcon_->RSSetState(nullptr);


}
void RenderingComponent::renderToBackBuffer()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };
	
	cbManagement_->getCBPerInstance()->update(devcon_, screenWidth_, screenHeight_, 32, 32);
	cbManagement_->getCBPerInstance()->csSet(devcon_);

	//Compute Shader
	ID3D11UnorderedAccessView* uav[] = { uavBackBuffer_ };
	devcon_->CSSetUnorderedAccessViews(0, 1, uav, nullptr);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers_[i]->getSRV();
	devcon_->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	devcon_->CSSetSamplers(0, 1, &ssDefault_);

	fxManagement_->getDefaultCS()->set(devcon_);
	devcon_->Dispatch(25, 25, 1);
	fxManagement_->getDefaultCS()->unset(devcon_);

	ID3D11UnorderedAccessView* uav2[] = { nullptr };
	devcon_->CSSetUnorderedAccessViews(0, 1, uav2, NULL);
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = nullptr;
	devcon_->CSSetShaderResources(0, GBUFFERID_NUM_BUFFERS, resourceViews);
	devcon_->CSSetSamplers(0, 0, nullptr);

	cbManagement_->getCBPerInstance()->csUnset(devcon_);

	swapChain_->Present(0, 0);
}

LPCWSTR RenderingComponent::featureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	LPCWSTR featureString = L"Default";
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		featureString = L"XKILL | Direct3D 11.0 device initiated with Direct3D 11.0 feature level";
	else if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		featureString = L"XKILL | Direct3D 11.0 device initiated with Direct3D 10.1 feature level";
	else if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		featureString = L"XKILL | Direct3D 11.0 device initiated with Direct3D 10.0 feature level";
	
	return featureString;
}
HRESULT RenderingComponent::initDeviceAndSwapChain()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount		= 1;
	swapChainDesc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width	= screenWidth_;
	swapChainDesc.BufferDesc.Height	= screenHeight_;
	swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.OutputWindow		= windowHandle_;
	swapChainDesc.SampleDesc.Count	= aliasingCount_;
	swapChainDesc.Windowed			= true;
	swapChainDesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = createDeviceAndSwapChain(swapChainDesc);

	return hr;
}
HRESULT RenderingComponent::createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc)
{
	HRESULT hr = E_FAIL;

	UINT numFeatureLevels = 3;
	D3D_FEATURE_LEVEL initiatedFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0,
										 D3D_FEATURE_LEVEL_10_1,
										 D3D_FEATURE_LEVEL_10_0};

	UINT numDriverTypes = 2;
	D3D_DRIVER_TYPE driverTypes[] = {	D3D_DRIVER_TYPE_HARDWARE,
										D3D_DRIVER_TYPE_REFERENCE};

	UINT flags = 0;
#if defined (_DEBUG) || defined (DEBUG)
	flags = D3D11_CREATE_DEVICE_DEBUG; //Enables shader debugging with PIX
#endif //_DEBUG || DEBUG

	unsigned int index = 0;
	while(index < numDriverTypes && hr != S_OK)
	{	
		D3D_DRIVER_TYPE driverType = driverTypes[index];

		hr = D3D11CreateDeviceAndSwapChain(0,
											driverType,
											0,
											flags,
											featureLevels,
											numFeatureLevels,
											D3D11_SDK_VERSION, 
											&swapChainDesc,
											&swapChain_,
											&device_,
											&initiatedFeatureLevel,
											&devcon_);
		if(SUCCEEDED(hr))
			SetWindowText(windowHandle_, featureLevelToString(initiatedFeatureLevel));

		index++;
	}
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::createDeviceAndSwapChain D3D11CreateDeviceAndSwapChain failed");

	return hr;
}
HRESULT RenderingComponent::initDepthBuffer()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));
	texd.Width		= screenWidth_;
	texd.Height		= screenHeight_;
	texd.ArraySize	= 1;
	texd.MipLevels	= 1;
	texd.SampleDesc.Count = aliasingCount_;
	texd.Format		= DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags	= D3D11_BIND_DEPTH_STENCIL;

	hr = device_->CreateTexture2D(&texd, NULL, &texDepthBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initDepthBuffer CreateTexture2D failed");

	if(hr == S_OK)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format			= DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2DMS;

		hr = device_->CreateDepthStencilView(texDepthBuffer_, &dsvd, &dsvDepthBuffer_);
		if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initDepthBuffer CreateDepthStencilView failed");
	}

	return hr;
}
HRESULT RenderingComponent::initBackBuffer()
{
	HRESULT hr = S_OK;

	swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texBackBuffer_);
	hr = device_->CreateRenderTargetView(texBackBuffer_, NULL, &rtvBackBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initBackBuffer CreateRenderTargetView failed");

	hr = device_->CreateUnorderedAccessView(texBackBuffer_, NULL, &uavBackBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initBackBuffer CreateUnorderedAccessView failed");

	return hr;
}
HRESULT RenderingComponent::initGBuffers()
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;

	/*Albedo*/
	gBuffer = new GBuffer(screenWidth_, screenHeight_, aliasingCount_, DXGI_FORMAT_R32G32B32A32_FLOAT);
	hr = gBuffer->init(device_);
	gBuffers_[GBUFFERID_ALBEDO] = gBuffer;

	/*Normals*/
	if(hr == S_OK)
	{
		gBuffer = new GBuffer(screenWidth_, screenHeight_, aliasingCount_, DXGI_FORMAT_R32G32B32A32_FLOAT);
		hr = gBuffer->init(device_);
		gBuffers_[GBUFFERID_NORMAL] = gBuffer;
	}

	return hr;
}
void RenderingComponent::initViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(screenWidth_);
	viewport.Height		= static_cast<FLOAT>(screenHeight_);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	devcon_->RSSetViewports(1, &viewport);
}
HRESULT RenderingComponent::initRSDefault()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode				= D3D11_CULL_NONE;
	rsd.FillMode				= D3D11_FILL_SOLID;
	rsd.FrontCounterClockwise	= false;
	rsd.DepthBias				= false;
	rsd.DepthBiasClamp			= 0;
	rsd.SlopeScaledDepthBias	= 0;
	rsd.DepthClipEnable			= true;
	rsd.ScissorEnable			= false;
	rsd.MultisampleEnable		= true;
	rsd.AntialiasedLineEnable	= true;

	hr = device_->CreateRasterizerState(&rsd, &rsDefault_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initRSDefault CreateRasterizerState failed");
	
	return hr;
}
HRESULT RenderingComponent::initSSDefault()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD		= 0;
    sampDesc.MaxLOD		= D3D11_FLOAT32_MAX;

	hr = device_->CreateSamplerState(&sampDesc, &ssDefault_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initSSDefault CreateSamplerState failed");

	return hr;
}
HRESULT RenderingComponent::initFXManagement()
{
	HRESULT hr = S_OK;

	fxManagement_ = new FXManagement();
	hr = fxManagement_->init(device_);

	return hr;
}
HRESULT RenderingComponent::initCBManagement()
{
	HRESULT hr = S_OK;

	cbManagement_ = new CBManagement();
	hr = cbManagement_->init(device_);

	return hr;
}
HRESULT RenderingComponent::initDebug()
{
	HRESULT hr = S_OK;

	d3dDebug_ = new D3DDebug();
	hr = d3dDebug_->init(device_);

	return hr;
}
HRESULT RenderingComponent::initVertexBuffer()
{
	HRESULT hr = S_OK;

	vertices_ = new std::vector<Vertex>();
	objLoader_ = new ObjLoaderBasic();
	objLoader_->parseObjectFile("../../xkill-resources/xkill-models/bth.obj", vertices_);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * vertices_->size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices_->at(0);
	device_->CreateBuffer(&vbd, &vinitData, &vertexBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initVertexBuffer CreateBuffer failed");

	return hr;
}