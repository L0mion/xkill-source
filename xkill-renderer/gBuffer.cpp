#include "gBuffer.h"
#include "renderingUtilities.h"

GBuffer::GBuffer(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat)
{
	this->texWidth_		= texWidth;
	this->texHeight_	= texHeight;
	this->texAliasing_	= texAliasing;
	this->texFormat_	= texFormat;

	tex_ = nullptr;
	srv_ = nullptr;
	rtv_ = nullptr;
}
GBuffer::~GBuffer()
{
	SAFE_RELEASE(tex_);
	SAFE_RELEASE(srv_);
	SAFE_RELEASE(rtv_);
}

void GBuffer::reset()
{
	SAFE_RELEASE(tex_);
	SAFE_RELEASE(srv_);
	SAFE_RELEASE(rtv_);
}

HRESULT GBuffer::resize(ID3D11Device* device, unsigned int texWidth, unsigned int texHeight)
{
	HRESULT hr = S_OK;
	
	reset();
	texWidth_	= texWidth;
	texHeight_	= texHeight;

	hr = init(device);

	return hr;
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
	descTex.Width		= texWidth_;
	descTex.Height		= texHeight_;
	descTex.MipLevels	= 1;
	descTex.ArraySize	= 1;
	descTex.Format		= texFormat_;
	descTex.SampleDesc.Count = texAliasing_;
	descTex.Usage		= D3D11_USAGE_DEFAULT;
	descTex.BindFlags	= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&descTex, NULL, &tex_);

	return hr;
}
HRESULT GBuffer::initSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format						= texFormat_;
	descSRV.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MostDetailedMip	= 0;
	descSRV.Texture2D.MipLevels			= 1;

	hr = device->CreateShaderResourceView(tex_, &descSRV, &srv_);

	return hr;
}
HRESULT GBuffer::initRTV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_RENDER_TARGET_VIEW_DESC descRTV;
	descRTV.Format				= texFormat_;
	descRTV.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	descRTV.Texture2D.MipSlice	= 0;
	
	hr = device->CreateRenderTargetView(tex_, &descRTV, &rtv_);

	return hr;
}

ID3D11ShaderResourceView* GBuffer::getSRV()
{
	return srv_;
}
ID3D11RenderTargetView* GBuffer::getRTV()
{
	return rtv_;
}

unsigned int GBuffer::getTexWidth() const
{
	return texWidth_;
}
unsigned int GBuffer::getTexHeight() const
{
	return texHeight_;
}