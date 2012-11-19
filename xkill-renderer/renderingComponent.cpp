#include <renderingComponent.h>
#include <fxManagement.h>

RenderingComponent::RenderingComponent(HWND windowHandle, UINT screenWidth, UINT screenHeight, UINT aliasingCount)
{
	this->windowHandle = windowHandle;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->aliasingCount = aliasingCount;

	device			= nullptr;
	devcon			= nullptr;

	fxManagement	= nullptr;

	swapChain		= nullptr;
	for(unsigned int i=0; i<2; i++)
		renderTargets[i] = nullptr;

	backBuffer			= nullptr;
	zBuffer				= nullptr;
	rasterizerState		= nullptr;
	defaultSamplerState = nullptr;

	normalSRV	= nullptr;
	diffuseSRV	= nullptr;

	backBufferTex	= nullptr;
	normalTex		= nullptr;
	diffuseTex		= nullptr;

	/*debug*/
	dxgiFactory		= nullptr;
	d3d11Debug		= nullptr;
	dxgiDebug		= nullptr;
	dxgiInfoQueue	= nullptr;
}
RenderingComponent::~RenderingComponent()
{
	if(fxManagement)
		delete fxManagement;

	if(swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
		swapChain->Release();
	}
	if(device)
		device->Release();
	if(devcon)
		devcon->Release();
	if(backBuffer)
		backBuffer->Release();
	if(zBuffer)
		zBuffer->Release();
	if(rasterizerState)
		rasterizerState->Release();
	if(defaultSamplerState)
		defaultSamplerState->Release();

	for(unsigned int i=0; i<2; i++)
	{
		if(renderTargets[i])
			renderTargets[i]->Release();
	}

	if(normalSRV)
		normalSRV->Release();
	if(diffuseSRV)
		diffuseSRV->Release();

	if(backBufferTex)
		backBufferTex->Release();
	if(normalTex)
		normalTex->Release();
	if(diffuseTex)
		diffuseTex->Release();

	/*debug*/
	if(dxgiInfoQueue)
		dxgiInfoQueue->Release();
	if(dxgiFactory)
		dxgiFactory->Release();
	if(d3d11Debug)
	{
		d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		d3d11Debug->Release();
	}
	if(dxgiDebug)
	{
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
		dxgiDebug->Release();
	}
}

HRESULT RenderingComponent::init()
{
	HRESULT hr = S_OK;

	if(hr == S_OK)
		hr = initDeviceAndSwapChain();
	if(hr == S_OK)
		hr = initDepthBuffer();
	if(hr == S_OK)
		hr = initRenderTargets();
	if(hr == S_OK)
		initViewport();
	if(hr == S_OK)
		hr = initRasterizerState();
	if(hr == S_OK)
		hr = initDefaultSamplerState();
	if(hr == S_OK)
		hr = initFXManagement();
	if(hr == S_OK)
		hr = initDebug();

	return hr;
}

void RenderingComponent::render()
{
	FLOAT black[]	= {0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT red[]		= {1.0f, 0.0f, 0.0f, 1.0f };
	FLOAT green[]	= {0.0f, 1.0f, 0.0f, 1.0f };
	FLOAT blue[]	= {0.0f, 0.0f, 1.0f, 1.0f };

	/*Render to G-Buffers*/
	devcon->VSSetShader(fxManagement->getDefaultVS(), nullptr, 0);
	devcon->PSSetShader(fxManagement->getDefaultPS(), nullptr, 0);
	devcon->PSSetSamplers(0, 1, &defaultSamplerState);
	devcon->OMSetRenderTargets(2, renderTargets, zBuffer); 
	devcon->ClearRenderTargetView(renderTargets[0], green);
	devcon->ClearRenderTargetView(renderTargets[1], blue);
	devcon->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	devcon->RSSetState(rasterizerState);
	devcon->IASetVertexBuffers(0, 0, NULL, 0, 0);
	devcon->IASetInputLayout(NULL);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devcon->Draw(4, 0);

	devcon->VSSetShader(NULL, NULL, 0);
	devcon->PSSetShader(NULL, NULL, 0);
	devcon->OMSetRenderTargets(0, NULL, NULL);


	/*Render to backbuffer*/
	devcon->VSSetShader(fxManagement->getDefaultVS(), nullptr, 0);
	devcon->PSSetShader(fxManagement->getDefaultDeferredPS(), nullptr, 0);
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);
	devcon->OMSetDepthStencilState(0, 0);
	devcon->ClearRenderTargetView(backBuffer, red);
	devcon->RSSetState(rasterizerState);
	devcon->IASetVertexBuffers(0, 0, NULL, 0, 0);
	devcon->IASetInputLayout(NULL);

	devcon->PSSetShaderResources(0, 1, &normalSRV);
	devcon->PSSetShaderResources(1, 1, &diffuseSRV);

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

HRESULT RenderingComponent::initDebug()
{
	HRESULT hr = S_OK;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&dxgiFactory));
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**); 
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll"); 
    fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
	
	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&dxgiDebug);
	DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&dxgiInfoQueue);

	device->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3d11Debug));

	return hr;
}

HRESULT RenderingComponent::initDeviceAndSwapChain()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.SampleDesc.Count = aliasingCount;
	swapChainDesc.Windowed = true;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

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

	return hr;
}

HRESULT RenderingComponent::initDepthBuffer()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = screenWidth;
	texd.Height = screenHeight;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = aliasingCount;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *depthBuffer;
	hr = device->CreateTexture2D(&texd, NULL, &depthBuffer);

	if(hr == S_OK)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

		hr = device->CreateDepthStencilView(depthBuffer, &dsvd, &zBuffer);
		depthBuffer->Release();
	}

	return hr;
}

HRESULT RenderingComponent::initRenderTargets()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width = screenWidth;
	desc.Height = screenHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = aliasingCount;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	device->CreateTexture2D(&desc, NULL, &backBufferTex);
	device->CreateTexture2D(&desc, NULL, &normalTex);
	device->CreateTexture2D(&desc, NULL, &diffuseTex);

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	device->CreateRenderTargetView(backBufferTex, NULL, &backBuffer);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	
	device->CreateRenderTargetView(normalTex, &rtvDesc, &renderTargets[0]);
	device->CreateRenderTargetView(diffuseTex, NULL, &renderTargets[1]);

	devcon->OMSetRenderTargets(2, renderTargets, zBuffer);

	/*Init SRVs*/
	initShaderResourceViews(desc);

	return hr;
}

void RenderingComponent::initViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(screenWidth);
	viewport.Height = static_cast<FLOAT>(screenHeight);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	devcon->RSSetViewports(1, &viewport);
}

HRESULT RenderingComponent::initRasterizerState()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode = D3D11_CULL_NONE;
	rsd.FillMode = D3D11_FILL_SOLID;
	rsd.FrontCounterClockwise = false;
	rsd.DepthBias = false;
	rsd.DepthBiasClamp = 0;
	rsd.SlopeScaledDepthBias = 0;
	rsd.DepthClipEnable = true;
	rsd.ScissorEnable = false;
	rsd.MultisampleEnable = true;
	rsd.AntialiasedLineEnable = true;

	hr = device->CreateRasterizerState(&rsd, &rasterizerState);
	devcon->RSSetState(rasterizerState);
	
	return hr;
}

HRESULT RenderingComponent::initDefaultSamplerState()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    
	sampDesc.Filter	= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD	= 0;
    sampDesc.MaxLOD	= D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &defaultSamplerState);

	return hr;
}

HRESULT RenderingComponent::initShaderResourceViews(D3D11_TEXTURE2D_DESC desc)
{
	HRESULT hr = S_OK;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format						= desc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	srvDesc.Texture2D.MipLevels			= 1;

	hr = device->CreateShaderResourceView(normalTex, &srvDesc, &normalSRV);
	if(hr == S_OK)
		hr = device->CreateShaderResourceView(diffuseTex, &srvDesc, &diffuseSRV);

	return hr;
}

HRESULT RenderingComponent::initFXManagement()
{
	HRESULT hr = S_OK;

	fxManagement = new FXManagement();
	hr = fxManagement->init(device);

	return hr;
}