#include "Buffer_SrvRtvUav.h"
#include "renderingUtilities.h"

Buffer_SrvRtvUav::Buffer_SrvRtvUav(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat,
	UINT			texBindFlags,
	D3D11_USAGE		texUsage) : Buffer_SrvRtv(texWidth, texHeight, texAliasing, texFormat, texBindFlags, texUsage)
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
HRESULT Buffer_SrvRtvUav::initUAV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format				= getTexFormat();
	uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice	= 0;

	hr = device->CreateUnorderedAccessView(tex_, &uavDesc, &uav_);

	return hr;
}