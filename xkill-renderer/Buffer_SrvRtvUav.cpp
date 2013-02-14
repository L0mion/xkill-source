#include "Buffer_SrvRtvUav.h"
#include "renderingUtilities.h"

Buffer_SrvRtvUav::Buffer_SrvRtvUav(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat,
	bool			maintainGivenResolution) : Buffer_SrvRtv(texWidth, texHeight, texAliasing, texFormat, maintainGivenResolution)
{
	uav_ = nullptr;
}

Buffer_SrvRtvUav::~Buffer_SrvRtvUav()
{
	SAFE_RELEASE(uav_);
}

void Buffer_SrvRtvUav::reset()
{
	SAFE_RELEASE(uav_);
	Buffer_SrvRtv::reset();
}

HRESULT Buffer_SrvRtvUav::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = Buffer_SrvRtv::init(device);
	if(SUCCEEDED(hr))
		hr = initUAV(device);

	return hr;
}
HRESULT Buffer_SrvRtvUav::initTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC descTex;
	ZeroMemory(&descTex, sizeof(descTex));
	descTex.Width		= texWidth_;
	descTex.Height		= texHeight_;
	descTex.MipLevels	= 1;
	descTex.ArraySize	= 1;
	descTex.Format		= texFormat_;
	descTex.SampleDesc.Count = texAliasing_;
	descTex.Usage		= D3D11_USAGE_DEFAULT;
	descTex.BindFlags	= 
		D3D11_BIND_RENDER_TARGET	| 
		D3D11_BIND_SHADER_RESOURCE	| 
		D3D11_BIND_UNORDERED_ACCESS;

	hr = device->CreateTexture2D(&descTex, NULL, &tex_);

	return hr;
}
HRESULT Buffer_SrvRtvUav::initUAV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	//Below settings are directly ripped from GBuffer_SrvRtv::initRTV. Not sure if these are correct of if more settings are required.
	//If this comment seemingly work - remove it.
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format				= texFormat_;
	uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice	= 0;

	hr = device->CreateUnorderedAccessView(tex_, &uavDesc, &uav_);

	return hr;
}

ID3D11UnorderedAccessView* Buffer_SrvRtvUav::getUAV()
{
	return uav_;
}