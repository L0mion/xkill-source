#include <d3d11.h>

#include "renderingUtilities.h"
#include "Buffer_SrvDsv.h"

Buffer_SrvDsv::Buffer_SrvDsv(
	unsigned int	texWidth,
	unsigned int	texHeight,
	unsigned int	texAliasing,
	DXGI_FORMAT		texFormat,
	UINT			texBindFlags,
	D3D11_USAGE		texUsage,

	DXGI_FORMAT srvFormat,
	
	DXGI_FORMAT dsvFormat) : Buffer_Srv(
	texWidth, 
	texHeight, 
	texAliasing, 
	texFormat, 
	texBindFlags, 
	texUsage, 
	
	srvFormat)
{
	dsvFormat_ = dsvFormat;

	//Default Values:
	dsv_ = nullptr;
}
Buffer_SrvDsv::~Buffer_SrvDsv()
{
	SAFE_RELEASE(dsv_);
}

void Buffer_SrvDsv::reset()
{
	SAFE_RELEASE(dsv_);
	Buffer_Srv::reset();
}

HRESULT Buffer_SrvDsv::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = Buffer_Srv::init(device);
	if(SUCCEEDED(hr))
		hr = initDSV(device);

	return hr;
}
HRESULT Buffer_SrvDsv::initDSV(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags				= 0;
	dsvDesc.Format				= dsvFormat_;
	dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS; //D3D11_DSV_DIMENSION_TEXTURE2D
	dsvDesc.Texture2D.MipSlice	= 0;

	hr = device->CreateDepthStencilView(
		tex_, 
		&dsvDesc, 
		&dsv_);

	return hr;
}