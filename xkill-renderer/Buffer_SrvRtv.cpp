#include "Buffer_SrvRtv.h"
#include "renderingUtilities.h"

Buffer_SrvRtv::Buffer_SrvRtv(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat,
	UINT			texBindFlags,
	D3D11_USAGE		texUsage) : Buffer_Srv(texWidth, texHeight, texAliasing, texFormat, texBindFlags, texUsage)
{
	rtv_ = nullptr;
}
Buffer_SrvRtv::~Buffer_SrvRtv()
{
	SAFE_RELEASE(rtv_);
}

void Buffer_SrvRtv::reset()
{
	SAFE_RELEASE(rtv_);
	Buffer_Srv::reset();
}

HRESULT Buffer_SrvRtv::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	Buffer_Srv::init(device);
	if(SUCCEEDED(hr))
		hr = initRTV(device);

	return hr;
}
HRESULT Buffer_SrvRtv::initRTV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_RENDER_TARGET_VIEW_DESC descRTV;
	descRTV.Format				= getTexFormat();
	descRTV.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	descRTV.Texture2D.MipSlice	= 0;
	
	hr = device->CreateRenderTargetView(tex_, &descRTV, &rtv_);

	return hr;
}