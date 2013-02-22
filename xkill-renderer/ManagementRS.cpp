#include "ManagementRS.h"

#include "renderingUtilities.h"

ManagementRS::ManagementRS()
{
	rsDefault_		= nullptr;
	rsWireframe_	= nullptr;
}
ManagementRS::~ManagementRS()
{
	SAFE_RELEASE(rsDefault_);
	SAFE_RELEASE(rsWireframe_);
}

void ManagementRS::reset()
{
	SAFE_RELEASE(rsDefault_);
	SAFE_RELEASE(rsWireframe_);
}

void ManagementRS::setRS(ID3D11DeviceContext* devcon, RS_ID rsId)
{
	switch(rsId)
	{
	case RS_ID_DEFAULT:
		devcon->RSSetState(rsDefault_);
		break;
	case RS_ID_WIREFRAME:
		devcon->RSSetState(rsWireframe_);
		break;
	default:
		devcon->RSSetState(rsDefault_);
		break;
	}
}

HRESULT ManagementRS::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initRSDefault(device);
	if(SUCCEEDED(hr))
		hr = initRSWireframe(device);

	return hr;
}
HRESULT ManagementRS::initRSDefault(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode				= D3D11_CULL_BACK;
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
HRESULT ManagementRS::initRSWireframe(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode				= D3D11_CULL_NONE;
	rsd.FillMode				= D3D11_FILL_WIREFRAME;
	rsd.FrontCounterClockwise	= false;
	rsd.DepthBias				= false;
	rsd.DepthBiasClamp			= 0;
	rsd.SlopeScaledDepthBias	= 0;
	rsd.DepthClipEnable			= true;
	rsd.ScissorEnable			= false;
	rsd.MultisampleEnable		= true;
	rsd.AntialiasedLineEnable	= true;

	hr = device->CreateRasterizerState(&rsd, &rsWireframe_);
	if(FAILED(hr))
		ERROR_MSG(L"RenderingComponent::initRSWireframe CreateRasterizerState failed");
	
	return hr;
}