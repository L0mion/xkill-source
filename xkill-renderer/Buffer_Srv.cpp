#include <d3d11.h>

#include "renderingUtilities.h"
#include "Buffer_Srv.h"

Buffer_Srv::Buffer_Srv(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	unsigned int	texMips,
	DXGI_FORMAT		texFormat,
	UINT			texBindFlags,
	D3D11_USAGE		texUsage)
{
	texWidth_		= texWidth;
	texHeight_		= texHeight;
	texAliasing_	= texAliasing;
	texMips_		= texMips;
	texFormat_		= texFormat;
	texBindFlags_	= texBindFlags;
	texUsage_		= texUsage;

	//Default values:
	srvFormat_ = texFormat;

	tex_ = nullptr;
	srv_ = nullptr;
}
Buffer_Srv::Buffer_Srv(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	unsigned int	texMips,
	DXGI_FORMAT		texFormat,
	UINT			texBindFlags,
	D3D11_USAGE		texUsage,

	DXGI_FORMAT srvFormat)
{
	texWidth_		= texWidth;
	texHeight_		= texHeight;
	texAliasing_	= texAliasing;
	texMips_		= texMips;
	texFormat_		= texFormat;
	texBindFlags_	= texBindFlags;
	texUsage_		= texUsage;
	
	srvFormat_ = srvFormat;
}
Buffer_Srv::~Buffer_Srv()
{
	SAFE_RELEASE(tex_);
	SAFE_RELEASE(srv_);
}

void Buffer_Srv::reset()
{
	SAFE_RELEASE(tex_);
	SAFE_RELEASE(srv_);
}

HRESULT Buffer_Srv::resize(ID3D11Device* device, unsigned int texWidth, unsigned int texHeight)
{
	HRESULT hr = S_OK;

	reset();
	texWidth_	= texWidth;
	texHeight_	= texHeight;

	hr = init(device);

	return hr;
}

HRESULT Buffer_Srv::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initTex(device);
	if(SUCCEEDED(hr))
		hr = initSRV(device);

	return hr;
}
HRESULT Buffer_Srv::initTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC descTex;
	ZeroMemory(&descTex, sizeof(descTex));
	descTex.Width				= texWidth_;
	descTex.Height				= texHeight_;
	descTex.MipLevels			= texMips_;
	descTex.ArraySize			= 1;
	descTex.Format				= texFormat_;
	descTex.SampleDesc.Count	= texAliasing_;
	descTex.Usage				= texUsage_;
	descTex.BindFlags			= texBindFlags_;

	hr = device->CreateTexture2D(
		&descTex, 
		NULL, //No initial data.
		&tex_);

	return hr;
}
HRESULT Buffer_Srv::initSRV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format						= srvFormat_;
	descSRV.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MostDetailedMip	= 0;
	descSRV.Texture2D.MipLevels			= 1;

	hr = device->CreateShaderResourceView(
		tex_, 
		&descSRV, 
		&srv_);

	return hr;
}