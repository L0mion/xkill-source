#include "RSManagement.h"

#include "renderingUtilities.h"

RSManagement::RSManagement()
{
	rsDefault_ = nullptr;
}
RSManagement::~RSManagement()
{
	SAFE_RELEASE(rsDefault_);
}

void RSManagement::reset()
{
	SAFE_RELEASE(rsDefault_);
}

void RSManagement::setRS(ID3D11DeviceContext* devcon, RS_ID rsId)
{
	switch(rsId)
	{
	case RS_ID_DEFAULT:
		devcon->RSSetState(rsDefault_);
		break;
	default:
		devcon->RSSetState(rsDefault_);
		break;
	}
}

HRESULT RSManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initRSDefault(device);

	return hr;
}
HRESULT RSManagement::initRSDefault(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode				= D3D11_CULL_NONE;
	rsd.FillMode				= D3D11_FILL_SOLID;
	rsd.FrontCounterClockwise	= false;
	rsd.DepthBias				= false;
	rsd.DepthBiasClamp			= 0;
	rsd.SlopeScaledDepthBias	= 0;
	rsd.DepthClipEnable			= true;
	rsd.ScissorEnable			= false;
	rsd.MultisampleEnable		= true;
	rsd.AntialiasedLineEnable	= true;

	hr = device->CreateRasterizerState(&rsd, &rsDefault_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initRSDefault CreateRasterizerState failed");
	
	return hr;
}