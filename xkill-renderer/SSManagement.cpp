
#include "SSManagement.h"
#include "renderingUtilities.h"

SSManagement::SSManagement()
{
	ssDefault_ = nullptr;
}
SSManagement::~SSManagement()
{
	SAFE_RELEASE(ssDefault_);
}

void SSManagement::reset()
{
	SAFE_RELEASE(ssDefault_);
}

void SSManagement::setVS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister)
{
	switch(ssId)
	{
	case SS_ID_DEFAULT:
		devcon->VSSetSamplers(shaderRegister, 1, &ssDefault_);
		break;
	default:
		devcon->VSSetSamplers(shaderRegister, 1, &ssDefault_);
		break;
	}
}
void SSManagement::setPS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister)
{
	switch(ssId)
	{
	case SS_ID_DEFAULT:
		devcon->PSSetSamplers(shaderRegister, 1, &ssDefault_);
		break;
	default:
		devcon->PSSetSamplers(shaderRegister, 1, &ssDefault_);
		break;
	}
}
void SSManagement::setCS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister)
{
	switch(ssId)
	{
	case SS_ID_DEFAULT:
		devcon->CSSetSamplers(shaderRegister, 1, &ssDefault_);
		break;
	default:
		devcon->CSSetSamplers(shaderRegister, 1, &ssDefault_);
		break;
	}
}

HRESULT SSManagement::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initSSDefault(device);

	return hr;
}
HRESULT SSManagement::initSSDefault(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD		= 0;
    sampDesc.MaxLOD		= D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &ssDefault_);
	if(FAILED(hr))
		ERROR_MSG(L"SSManagement::initSSDefault CreateSamplerState failed");

	return hr;
}