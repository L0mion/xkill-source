#include <renderingComponent.h>
#include <fxManagement.h>
#include <gBuffer.h>
#include <renderingUtilities.h>
#include <d3dDebug.h>
#include <constantBuffers.h>
#include <objLoaderBasic.h>

RenderingComponent::RenderingComponent(HWND windowHandle,
									   unsigned int screenWidth,
									   unsigned int screenHeight,
									   unsigned int aliasingCount)
{
	this->windowHandle	= windowHandle;
	this->screenWidth	= screenWidth;
	this->screenHeight	= screenHeight;
	this->aliasingCount	= aliasingCount;

	fxManagement	= nullptr;
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		gBuffers[i] = nullptr;
	d3dDebug		= nullptr;

	device	= nullptr;
	devcon	= nullptr;
	swapChain = nullptr;

	rtvBackBuffer	= nullptr;
	dsvDepthBuffer	= nullptr;
	rsDefault		= nullptr;
	ssDefault		= nullptr;
	
	texBackBuffer	= nullptr;
	texDepthBuffer	= nullptr;

	cbPerFrame	= nullptr;

	//temp
	vertexBuffer = nullptr;
	vertices = nullptr;
	objLoader = nullptr;
}
RenderingComponent::~RenderingComponent()
{
	if(swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
		SAFE_RELEASE(swapChain);
	}
	
	SAFE_RELEASE(device);
	SAFE_RELEASE(devcon);
	SAFE_RELEASE(rtvBackBuffer);
	SAFE_RELEASE(dsvDepthBuffer);
	SAFE_RELEASE(rsDefault);
	SAFE_RELEASE(ssDefault);
	SAFE_RELEASE(texBackBuffer);
	SAFE_RELEASE(texDepthBuffer);
	SAFE_RELEASE(cbPerFrame);

	SAFE_RELEASE(vertexBuffer); //temp

	SAFE_DELETE(fxManagement);
	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		SAFE_DELETE(gBuffers[i]);
	
	//d3dDebug->reportLiveDeviceObjects();
	SAFE_DELETE(d3dDebug);

	//temp
	SAFE_DELETE(vertices);
	SAFE_DELETE(objLoader);
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
		hr = initConstantBuffers();
	if(SUCCEEDED(hr)) //temp
		hr = initVertexBuffer();
	if(SUCCEEDED(hr))
		hr = initDebug();

	return hr;
}

void RenderingComponent::reset()
{
	if(swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
		SAFE_RELEASE(swapChain);
	}
	
	if(fxManagement)
		fxManagement->reset();

	for(unsigned int i = 0; i < GBUFFERID_NUM_BUFFERS; i++)
		if(gBuffers[i])
			gBuffers[i]->reset();
	
	SAFE_RELEASE(device);
	SAFE_RELEASE(devcon);
	SAFE_RELEASE(rtvBackBuffer);
	SAFE_RELEASE(dsvDepthBuffer);
	SAFE_RELEASE(rsDefault);
	SAFE_RELEASE(ssDefault);
	SAFE_RELEASE(texBackBuffer);
	SAFE_RELEASE(texDepthBuffer);
	SAFE_RELEASE(cbPerFrame);

	//temp
	SAFE_RELEASE(vertexBuffer);
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

	devcon->VSSetShader(fxManagement->getDefaultVS(), nullptr, 0);
	devcon->PSSetShader(fxManagement->getDefaultPS(), nullptr, 0);
	devcon->PSSetSamplers(0, 1, &ssDefault);

	CBPerFrame cbPerFrame;
	cbPerFrame.worldViewProj = view * projection;
	devcon->UpdateSubresource(this->cbPerFrame, 0, 0, &cbPerFrame, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &this->cbPerFrame);

	ID3D11RenderTargetView* renderTargets[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		renderTargets[i] = gBuffers[i]->getRTV();
	devcon->OMSetRenderTargets(GBUFFERID_NUM_BUFFERS, renderTargets, dsvDepthBuffer);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_ALBEDO], green);
	devcon->ClearRenderTargetView(renderTargets[GBUFFERID_NORMAL], blue);

	devcon->ClearDepthStencilView(dsvDepthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	devcon->RSSetState(rsDefault);
	//devcon->IASetVertexBuffers(0, 0, NULL, 0, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetInputLayout(fxManagement->getInputLayout());
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(vertices->size(), 0);

	devcon->VSSetShader(NULL, NULL, 0);
	devcon->PSSetShader(NULL, NULL, 0);
	devcon->OMSetRenderTargets(0, NULL, NULL);
}
void RenderingComponent::renderToBackBuffer()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	devcon->VSSetShader(fxManagement->getDefaultDeferredVS(), nullptr, 0);
	devcon->PSSetShader(fxManagement->getDefaultDeferredPS(), nullptr, 0);
	devcon->OMSetRenderTargets(1, &rtvBackBuffer, NULL);
	devcon->OMSetDepthStencilState(0, 0);
	devcon->ClearRenderTargetView(rtvBackBuffer, red);
	devcon->RSSetState(rsDefault);
	devcon->IASetVertexBuffers(0, 0, NULL, 0, 0);
	devcon->IASetInputLayout(NULL);

	ID3D11ShaderResourceView* resourceViews[GBUFFERID_NUM_BUFFERS];
	for(int i=0; i<GBUFFERID_NUM_BUFFERS; i++)
		resourceViews[i] = gBuffers[i]->getSRV();
	devcon->PSSetShaderResources(0, 2, resourceViews);
	
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devcon->Draw(4, 0);
	
	ID3D11ShaderResourceView* resetSRV[2] = {NULL};
	devcon->PSSetShaderResources(0, 2, resetSRV);
	devcon->VSSetShader(NULL, NULL, 0);
	devcon->PSSetShader(NULL, NULL, 0);
	devcon->OMSetRenderTargets(0, NULL, NULL);
	
	swapChain->Present(0, 0);
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
	swapChainDesc.BufferDesc.Width	= screenWidth;
	swapChainDesc.BufferDesc.Height	= screenHeight;
	swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow		= windowHandle;
	swapChainDesc.SampleDesc.Count	= aliasingCount;
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
											&swapChain,
											&device,
											&initiatedFeatureLevel,
											&devcon);
		if(SUCCEEDED(hr))
			SetWindowText(windowHandle, featureLevelToString(initiatedFeatureLevel));

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
	texd.Width		= screenWidth;
	texd.Height		= screenHeight;
	texd.ArraySize	= 1;
	texd.MipLevels	= 1;
	texd.SampleDesc.Count = aliasingCount;
	texd.Format		= DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags	= D3D11_BIND_DEPTH_STENCIL;

	hr = device->CreateTexture2D(&texd, NULL, &texDepthBuffer);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initDepthBuffer CreateTexture2D failed");

	if(hr == S_OK)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format			= DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2DMS;

		hr = device->CreateDepthStencilView(texDepthBuffer, &dsvd, &dsvDepthBuffer);
		if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initDepthBuffer CreateDepthStencilView failed");
	}

	return hr;
}
HRESULT RenderingComponent::initBackBuffer()
{
	HRESULT hr = S_OK;

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texBackBuffer);
	hr = device->CreateRenderTargetView(texBackBuffer, NULL, &rtvBackBuffer);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initBackBuffer CreateRenderTargetView failed");

	return hr;
}
HRESULT RenderingComponent::initGBuffers()
{
	HRESULT hr = S_OK;

	GBuffer* gBuffer = nullptr;

	/*Albedo*/
	gBuffer = new GBuffer(screenWidth, screenHeight, aliasingCount, DXGI_FORMAT_R32G32B32A32_FLOAT);
	hr = gBuffer->init(device);
	gBuffers[GBUFFERID_ALBEDO] = gBuffer;

	/*Normals*/
	if(hr == S_OK)
	{
		gBuffer = new GBuffer(screenWidth, screenHeight, aliasingCount, DXGI_FORMAT_R32G32B32A32_FLOAT);
		hr = gBuffer->init(device);
		gBuffers[GBUFFERID_NORMAL] = gBuffer;
	}

	return hr;
}
void RenderingComponent::initViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<FLOAT>(screenWidth);
	viewport.Height		= static_cast<FLOAT>(screenHeight);
	viewport.MinDepth	= 0;
	viewport.MaxDepth	= 1;

	devcon->RSSetViewports(1, &viewport);
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

	hr = device->CreateRasterizerState(&rsd, &rsDefault);
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

	hr = device->CreateSamplerState(&sampDesc, &ssDefault);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initSSDefault CreateSamplerState failed");

	return hr;
}
HRESULT RenderingComponent::initFXManagement()
{
	HRESULT hr = S_OK;

	fxManagement = new FXManagement();
	hr = fxManagement->init(device);

	return hr;
}
HRESULT RenderingComponent::initDebug()
{
	HRESULT hr = S_OK;

	d3dDebug = new D3DDebug();
	hr = d3dDebug->init(device);

	return hr;
}
HRESULT RenderingComponent::initConstantBuffers()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 64; //Must be a multiple of 16 bytes for constant buffers!
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	
	hr = device->CreateBuffer(&bd, NULL, &cbPerFrame);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initConstantBuffers CreateBuffer failed");

	return hr;
}
HRESULT RenderingComponent::initVertexBuffer()
{
	HRESULT hr = S_OK;

	vertices = new std::vector<Vertex>();
	objLoader = new ObjLoaderBasic();
	objLoader->parseObjectFile("../../xkill-resources/xkill-models/bth.obj", vertices);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * vertices->size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices->at(0);
	device->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initVertexBuffer CreateBuffer failed");

	return hr;
}