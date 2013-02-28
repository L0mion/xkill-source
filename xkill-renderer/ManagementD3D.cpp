#include <DirectXMath.h>
#include <comdef.h>

#include "renderingUtilities.h"
#include "Winfo.h"
#include "Buffer_SrvDsv.h"
#include "Buffer_SrvRtvUav.h"
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
	
	depthBuffer_ = nullptr;

	texBackBuffer_	= nullptr;
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

	SAFE_DELETE(depthBuffer_);

	SAFE_RELEASE(texBackBuffer_);
}
void ManagementD3D::reset()
{
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(swapChain_);
	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);

	SAFE_RESET(depthBuffer_);

	SAFE_RELEASE(texBackBuffer_);
}

HRESULT ManagementD3D::resize()
{
	HRESULT hr = S_OK;

	SAFE_RELEASE(rtvBackBuffer_);
	SAFE_RELEASE(uavBackBuffer_);
	SAFE_RELEASE(texBackBuffer_);

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
		hr = depthBuffer_->resize(device_, winfo_->getScreenWidth(), winfo_->getScreenHeight()); //hr = initDepthBuffer();

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
										 D3D_FEATURE_LEVEL_10_0,
										 D3D_FEATURE_LEVEL_9_3,
										 D3D_FEATURE_LEVEL_9_2,
										 D3D_FEATURE_LEVEL_9_1};

	UINT numDriverTypes = 2;
	D3D_DRIVER_TYPE driverTypes[] = {	D3D_DRIVER_TYPE_HARDWARE,
										D3D_DRIVER_TYPE_REFERENCE};

	UINT flags = 0;

#if defined(D3D_PROFILE)
	flags |= D3D11_CREATE_DEVICE_DEBUG; //Enables shader debugging
#endif //D3D_PROFILE

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
	if(initiatedFeatureLevel == D3D_FEATURE_LEVEL_9_3 ||
	   initiatedFeatureLevel == D3D_FEATURE_LEVEL_9_2 ||
	   initiatedFeatureLevel == D3D_FEATURE_LEVEL_9_1)
	{
		ERROR_MSG(L"DirectX Device initiated with DirectX 9.x feature level");
	}

	if(FAILED(hr))
	{
		std::string message;

		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();

		ERROR_MSG(errMsg);
		//ERROR_MSG(L"RenderingComponent::createDeviceAndSwapChain D3D11CreateDeviceAndSwapChain failed.");
	}
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

	depthBuffer_ = new Buffer_SrvDsv(
		winfo_->getScreenWidth(),
		winfo_->getScreenHeight(),
		MULTISAMPLES_DEPTHBUFFER,
		DXGI_FORMAT_R32_TYPELESS,
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT);
	hr = depthBuffer_->init(device_);

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
	devcon_->CSSetUnorderedAccessViews(
		0, 
		1, 
		&uavBackBuffer_, 
		nullptr);
}
void ManagementD3D::unsetUAVBackBufferCS()
{
	ID3D11UnorderedAccessView* uav[] = { nullptr };
	devcon_->CSSetUnorderedAccessViews(
		0, 
		1, 
		uav, 
		nullptr);
}
void ManagementD3D::setDepthBufferSRV(unsigned int shaderRegister)
{
	ID3D11ShaderResourceView* depthSRV = depthBuffer_->getSRV();
	devcon_->CSSetShaderResources(
		shaderRegister, 
		1, 
		&depthSRV); //&srvDepthBuffer_);
}
void ManagementD3D::unsetDepthBufferSRV(unsigned int shaderRegister)
{
	ID3D11ShaderResourceView* nullViews[1] = { nullptr };

	devcon_->CSSetShaderResources(
		shaderRegister,
		1,
		nullViews);
}
void ManagementD3D::setRenderTargetViewBackBuffer()
{
	ID3D11DepthStencilView* depthDSV = depthBuffer_->getDSV();
	devcon_->OMSetRenderTargets(
		1, 
		&rtvBackBuffer_, 
		depthDSV); //dsvDepthBuffer_);
}
void ManagementD3D::unsetRenderTargetViewBackBuffer()
{
//	devcon_->OMSetRenderTargets(1, nullptr, nullptr);
}

void ManagementD3D::present()
{
	swapChain_->Present(0, 0);
}
void ManagementD3D::clearDepthBuffer()
{
	ID3D11DepthStencilView* depthDSV = depthBuffer_->getDSV();
	devcon_->ClearDepthStencilView(
		depthDSV, //dsvDepthBuffer_, 
		D3D11_CLEAR_DEPTH, 
		1.0f, 
		0);
}
void ManagementD3D::clearBackBuffer()
{
	devcon_->ClearUnorderedAccessViewFloat(uavBackBuffer_, CLEAR_COLOR_BACKBUFFER_BLACK);
}

ID3D11Device*				ManagementD3D::getDevice()			const
{
	return device_;
}
ID3D11DeviceContext*		ManagementD3D::getDeviceContext()	const
{
	return devcon_;
}
ID3D11DepthStencilView*		ManagementD3D::getDepthBuffer()		const
{
	return depthBuffer_->getDSV(); //dsvDepthBuffer_;
}
ID3D11Texture2D*			ManagementD3D::getDepthBufferTexture() const
{
	return depthBuffer_->getTex(); //texDepthBuffer_;
}
ID3D11ShaderResourceView*	ManagementD3D::getDepthBufferSRV()		const
{
	return depthBuffer_->getSRV();
}