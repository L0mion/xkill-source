#include "gBuffer.h"
#include "renderingUtilities.h"

GBuffer::GBuffer(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat)
{
	this->texWidth		= texWidth;
	this->texHeight		= texHeight;
	this->texAliasing	= texAliasing;
	this->texFormat		= texFormat;

	tex = nullptr;
	srv = nullptr;
	rtv = nullptr;
}
GBuffer::~GBuffer()
{
	SAFE_RELEASE(tex);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtv);
}

void GBuffer::reset()
{
	SAFE_RELEASE(tex);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtv);
}
HRESULT GBuffer::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initTex(device);
	if(hr == S_OK)
		hr = initSRV(device);
	if(hr == S_OK)
		hr = initRTV(device);

	return hr;
}
HRESULT GBuffer::initTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC descTex;
	ZeroMemory(&descTex, sizeof(descTex));
	descTex.Width		= texWidth;
	descTex.Height		= texHeight;
	descTex.MipLevels	= 1;
	descTex.ArraySize	= 1;
	descTex.Format		= texFormat;
	descTex.SampleDesc.Count = texAliasing;
	descTex.Usage		= D3D11_USAGE_DEFAULT;
	descTex.BindFlags	= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&descTex, NULL, &tex);

	return hr;
}
HRESULT GBuffer::initSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format						= texFormat;
	descSRV.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MostDetailedMip	= 0;
	descSRV.Texture2D.MipLevels			= 1;

	hr = device->CreateShaderResourceView(tex, &descSRV, &srv);

	return hr;
}
HRESULT GBuffer::initRTV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_RENDER_TARGET_VIEW_DESC descRTV;
	descRTV.Format				= texFormat;
	descRTV.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	descRTV.Texture2D.MipSlice	= 0;
	
	hr = device->CreateRenderTargetView(tex, &descRTV, &rtv);

	return hr;
}

ID3D11ShaderResourceView* GBuffer::getSRV()
{
	return srv;
}

ID3D11RenderTargetView* GBuffer::getRTV()
{
	return rtv;
}