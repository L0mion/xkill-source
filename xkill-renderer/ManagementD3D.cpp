#include <DirectXMath.h>

#include "renderingUtilities.h"
#include "Winfo.h"
#include "ManagementD3D.h"

ManagementD3D::ManagementD3D(HWND windowHandle, Winfo* winfo)
{
	windowHandle_	= windowHandle;
	winfo_			= winfo;

	device_		= nullptr;
	devcon_		= nullptr;
	swapChain_	= nullptr;

	rtvBackBuffer_	= nullptr;
	uavBackBuffer_	= nullptr;
	dsvDepthBuffer_	= nullptr;
	
	texBackBuffer_	= nullptr;
	texDepthBuffer_	= nullptr;
}
ManagementD3D::~ManagementD3D()
{
	if(swapChain_)
		swapChain_->SetFullscreenState(false, nullptr);
	SAFE_RELEASE(swapChain_);

	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);

	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);
}
void ManagementD3D::reset()
{
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(swapChain_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);

	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);
}

HRESULT ManagementD3D::resize()
{
	HRESULT hr = S_OK;

	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(dsvDepthBuffer_);
	SAFE_RELEASE(texBackBuffer_);
	SAFE_RELEASE(texDepthBuffer_);

	devcon_->OMSetRenderTargets(0, 0, 0);

	hr = swapChain_->ResizeBuffers(
		1, 
		winfo_->getScreenWidth(), 
		winfo_->getScreenHeight(), 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		0);
	if(FAILED(hr))
		ERROR_MSG(L"D3DManagement::resize | swapChain_->ResizeBuffers | Failed!");

	if(SUCCEEDED(hr))
		hr = initBackBuffer();
	if(SUCCEEDED(hr))
		hr = initDepthBuffer();

	return hr;
}

HRESULT ManagementD3D::init()
{
	HRESULT hr = S_OK;

	if(SUCCEEDED(hr))
		hr = initDeviceAndSwapChain();
	if(SUCCEEDED(hr))
		hr = initBackBuffer();
	if(SUCCEEDED(hr))
		hr = initDepthBuffer();

	return hr;
}
HRESULT ManagementD3D::initDeviceAndSwapChain()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount		= 1;
	swapChainDesc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width	= winfo_->getScreenWidth();
	swapChainDesc.BufferDesc.Height	= winfo_->getScreenHeight();
	swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.OutputWindow		= windowHandle_;
	swapChainDesc.SampleDesc.Count	= MULTISAMPLES_BACKBUFFER;
	swapChainDesc.Windowed			= true;
	swapChainDesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = createDeviceAndSwapChain(swapChainDesc);

	return hr;
}
HRESULT ManagementD3D::createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC swapChainDesc)
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
HRESULT ManagementD3D::initBackBuffer()
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
HRESULT ManagementD3D::initDepthBuffer()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));
	texd.Width		= winfo_->getScreenWidth();
	texd.Height		= winfo_->getScreenHeight();
	texd.ArraySize	= 1;
	texd.MipLevels	= 1;
	texd.SampleDesc.Count = MULTISAMPLES_DEPTHBUFFER;
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
LPCWSTR ManagementD3D::featureLevelToString(D3D_FEATURE_LEVEL featureLevel)
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

void ManagementD3D::setUAVBackBufferCS()
{
	devcon_->CSSetUnorderedAccessViews(0, 1, &uavBackBuffer_, nullptr);
}
void ManagementD3D::setRenderTargetViewBackBuffer()
{
	devcon_->OMSetRenderTargets(1, &rtvBackBuffer_, dsvDepthBuffer_);
}
void ManagementD3D::unsetRenderTargetViewBackBufer()
{
//	devcon_->OMSetRenderTargets(1, nullptr, nullptr);
}

void ManagementD3D::present()
{
	swapChain_->Present(0, 0);
}
void ManagementD3D::clearDepthBuffer()
{
	devcon_->ClearDepthStencilView(dsvDepthBuffer_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11Device*			ManagementD3D::getDevice()			const
{
	return device_;
}
ID3D11DeviceContext*	ManagementD3D::getDeviceContext()	const
{
	return devcon_;
}
ID3D11DepthStencilView* ManagementD3D::getDepthBuffer()		const
{
	return dsvDepthBuffer_;
}