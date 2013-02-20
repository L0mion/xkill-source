#include "Buffer_SrvRtv.h"
#include "renderingUtilities.h"

Buffer_SrvRtv::Buffer_SrvRtv(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat) : Buffer_Srv(texWidth, texHeight, texAliasing, texFormat)
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
	if(hr == S_OK)
		hr = initRTV(device);

	return hr;
}
HRESULT Buffer_SrvRtv::initTex(ID3D11Device* device)
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
		D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&descTex, NULL, &tex_);

	return hr;
}
HRESULT Buffer_SrvRtv::initRTV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_RENDER_TARGET_VIEW_DESC descRTV;
	descRTV.Format				= texFormat_;
	descRTV.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	descRTV.Texture2D.MipSlice	= 0;
	
	hr = device->CreateRenderTargetView(tex_, &descRTV, &rtv_);

	return hr;
}