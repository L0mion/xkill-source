
#include "ManagementSS.h"
#include "renderingUtilities.h"

ManagementSS::ManagementSS()
{
	ssDefault_	= nullptr;
	ssShadow_	= nullptr;

	ssNormal_	= nullptr;
	ssDepth_	= nullptr;
	ssRandom_	= nullptr;
}
ManagementSS::~ManagementSS()
{
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(ssShadow_);

	SAFE_RELEASE(ssNormal_);
	SAFE_RELEASE(ssDepth_);
	SAFE_RELEASE(ssRandom_);
}

void ManagementSS::reset()
{
	SAFE_RELEASE(ssDefault_);
	SAFE_RELEASE(ssShadow_);

	SAFE_RELEASE(ssNormal_);
	SAFE_RELEASE(ssDepth_);
	SAFE_RELEASE(ssRandom_);
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
	case SS_ID_SHADOW:
		ss = ssShadow_;
		break;
	case SS_ID_NORMAL:
		ss = ssNormal_;
		break;
	case SS_ID_DEPTH:
		ss = ssDepth_;
		break;
	case SS_ID_RANDOM:
		ss = ssRandom_;
		break;
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
	if(SUCCEEDED(hr))
		hr = initSSShadow(device);
	if(SUCCEEDED(hr))
		hr = initSSNormal(device);
	if(SUCCEEDED(hr))
		hr = initSSDepth(device);
	if(SUCCEEDED(hr))
		hr = initSSRandom(device);

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
HRESULT ManagementSS::initSSShadow(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC ssDesc;
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.Filter			= D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	ssDesc.AddressU			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.ComparisonFunc	= D3D11_COMPARISON_LESS_EQUAL;
	ssDesc.MinLOD			= 0;
	ssDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	ssDesc.BorderColor[0] = 0.0f;
	ssDesc.BorderColor[1] = 0.0f;
	ssDesc.BorderColor[2] = 0.0f;
	ssDesc.BorderColor[3] = 0.0f;

	hr = device->CreateSamplerState(&ssDesc, &ssShadow_);
	if(FAILED(hr))
		ERROR_MSG(L"SSManagement::initSSShadow CreateSamplerState failed");

	return hr;
}
HRESULT ManagementSS::initSSNormal(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC ssDesc;
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.Filter			= D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	ssDesc.AddressU			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	ssDesc.MinLOD			= 0;
	ssDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	ssDesc.BorderColor[0] = 0.0f;
	ssDesc.BorderColor[1] = 0.0f;
	ssDesc.BorderColor[2] = 0.0f;
	ssDesc.BorderColor[3] = 0.0f;

	hr = device->CreateSamplerState(&ssDesc, &ssNormal_);
	if(FAILED(hr))
		ERROR_MSG(L"SSManagement::initSSNormal CreateSamplerState failed");

	return hr;
}
HRESULT ManagementSS::initSSDepth(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC ssDesc;
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.Filter			= D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	ssDesc.AddressU			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;
	ssDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	ssDesc.MinLOD			= 0;
	ssDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	ssDesc.BorderColor[0] = 1e5f;
	ssDesc.BorderColor[1] = 1e5f;
	ssDesc.BorderColor[2] = 1e5f;
	ssDesc.BorderColor[3] = 1e5f;

	hr = device->CreateSamplerState(&ssDesc, &ssDepth_);
	if(FAILED(hr))
		ERROR_MSG(L"SSManagement::initSSDepth CreateSamplerState failed");

	return hr;
}
HRESULT ManagementSS::initSSRandom(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC ssDesc;
	ZeroMemory(&ssDesc, sizeof(ssDesc));
	ssDesc.Filter			= D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	ssDesc.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;
	ssDesc.AddressV			= D3D11_TEXTURE_ADDRESS_CLAMP;
	ssDesc.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
	ssDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	ssDesc.MinLOD			= 0;
	ssDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&ssDesc, &ssRandom_);
	if(FAILED(hr))
		ERROR_MSG(L"SSManagement::initSSRandom CreateSamplerState failed");

	return hr;
}