
#include "ManagementSS.h"
#include "renderingUtilities.h"

ManagementSS::ManagementSS()
{
	ssDefault_ = nullptr;
	ssSprite_  = nullptr;
}
ManagementSS::~ManagementSS()
{
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(ssSprite_);
}

void ManagementSS::reset()
{
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(ssSprite_);
}

void ManagementSS::setSS(
	ID3D11DeviceContext*	devcon,
	TypeFX					shaderStage,
	unsigned int			shaderRegister,
	SS_ID					ssId)
{
	//Get specified ss
	ID3D11SamplerState* ss = nullptr;
	switch(ssId)
	{
	case SS_ID_DEFAULT:
		ss = ssDefault_;
		break;
	case SS_ID_SPRITE:
		ss = ssSprite_;
	default:
		ss = ssDefault_; // Should we really set a 'default' samplerstate this way?
		break;
	}

	//Bind to specified shade-stage
	switch(shaderStage)
	{
	case TypeFX_VS:
		devcon->VSSetSamplers(shaderRegister, 1, &ss);
		break;
	case TypeFX_GS:
		devcon->GSSetSamplers(shaderRegister, 1, &ss);
		break;
	case TypeFX_HS:
		devcon->HSSetSamplers(shaderRegister, 1, &ss);
		break;
	case TypeFX_DS:
		devcon->DSSetSamplers(shaderRegister, 1, &ss);
		break;
	case TypeFX_PS:
		devcon->PSSetSamplers(shaderRegister, 1, &ss);
		break;
	case TypeFX_CS:
		devcon->CSSetSamplers(shaderRegister, 1, &ss);
		break;
	};
}
void ManagementSS::unsetSS(
	ID3D11DeviceContext*	devcon,
	TypeFX					shaderStage,
	unsigned int			shaderRegister)
{
	switch(shaderStage)
	{
	case TypeFX_VS:
		devcon->VSSetSamplers(shaderRegister, 0, nullptr);
		break;
	case TypeFX_GS:
		devcon->GSSetSamplers(shaderRegister, 0, nullptr);
		break;
	case TypeFX_HS:
		devcon->HSSetSamplers(shaderRegister, 0, nullptr);
		break;
	case TypeFX_DS:
		devcon->DSSetSamplers(shaderRegister, 0, nullptr);
		break;
	case TypeFX_PS:
		devcon->PSSetSamplers(shaderRegister, 0, nullptr);
		break;
	case TypeFX_CS:
		devcon->CSSetSamplers(shaderRegister, 0, nullptr);
		break;
	};
}

HRESULT ManagementSS::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initSSDefault(device);

	return hr;
}
HRESULT ManagementSS::initSSDefault(ID3D11Device* device)
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

HRESULT ManagementSS::initSSSprite(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter		= D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    sampDesc.AddressU	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW	= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD		= 0;
    sampDesc.MaxLOD		= D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &ssSprite_);
	if(FAILED(hr))
		ERROR_MSG(L"SSManagement::initSSCrossHair CreateSamplerState failed");

	return hr;
}