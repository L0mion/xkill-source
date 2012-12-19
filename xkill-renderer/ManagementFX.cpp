//#include <d3d11.h>
//#include <d3d10.h>
#include <d3dcompiler.h>

#include "renderingUtilities.h"
#include "ManagementFX.h"

ManagementFX::ManagementFX(bool debugShaders)
{
	debugShaders_ = debugShaders;

	managementIED_ = nullptr;

	defaultVS_				= nullptr;
	defaultPS_				= nullptr;
	defaultDeferredVS_		= nullptr;
	defaultDeferredPS_		= nullptr;
	defaultCS_				= nullptr;
	animationVS_			= nullptr;
	animationPS_			= nullptr;
	colorVS_				= nullptr;
	colorPS_				= nullptr;

	ilPosColor_				= nullptr;
	ilPosNormTex_			= nullptr;
	ilPosNormTexTanSkinned_ = nullptr;
}
ManagementFX::~ManagementFX()
{
	SAFE_DELETE(managementIED_);

	SAFE_DELETE(defaultVS_);
	SAFE_DELETE(defaultPS_);
	SAFE_DELETE(defaultDeferredVS_);
	SAFE_DELETE(defaultDeferredPS_);
	SAFE_DELETE(defaultCS_);
	SAFE_DELETE(animationVS_);
	SAFE_DELETE(animationPS_);
	SAFE_DELETE(colorVS_);
	SAFE_DELETE(colorPS_);

	SAFE_RELEASE(ilPosColor_);
	SAFE_RELEASE(ilPosNormTex_);
	SAFE_RELEASE(ilPosNormTexTanSkinned_);
}

void ManagementFX::reset()
{
	defaultVS_->reset();
	defaultPS_->reset();
	defaultDeferredVS_->reset();
	defaultDeferredPS_->reset();
	defaultCS_->reset();
	animationVS_->reset();
	animationPS_->reset();
	
	SAFE_RELEASE(ilPosNormTex_);
}

HRESULT ManagementFX::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initShaders(device);
	if(SUCCEEDED(hr))
		hr = initILs(device);

	return hr;
}

void ManagementFX::setShader(ID3D11DeviceContext* devcon, ShaderID shaderID)
{
	Shader* shader = getShaderFromID(shaderID);
	if(shader)
		shader->set(devcon);
}
void ManagementFX::unsetShader(ID3D11DeviceContext* devcon, ShaderID shaderID)
{
	Shader* shader = getShaderFromID(shaderID);
	if(shader)
		shader->unset(devcon);
}
void ManagementFX::setLayout(ID3D11DeviceContext* devcon,	LayoutID layoutID)
{
	ID3D11InputLayout* il = nullptr;
	switch(layoutID)
	{
	case LAYOUTID_POS_NORM_TEX:
		il = ilPosNormTex_;
		break;
	case LAYOUTID_POS_NORM_TEX_TAN_SKINNED:
		il = ilPosNormTexTanSkinned_;
		break;
	};

	if(il)
		devcon->IASetInputLayout(il);
}

HRESULT ManagementFX::initShaders(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	std::wstring shaderPath;
	if(debugShaders_)
		shaderPath = L"../../xkill-build/bin-Debug/";
	else
		shaderPath = L"../../xkill-build/bin-Release/";

	hr = initDefaultVS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initDefaultPS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initDefaultDeferredVS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initDefaultDeferredPS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initDefaultCS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initAnimationVS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initAnimationPS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initColorVS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initColorPS(device, shaderPath);
	
	return hr;
}
HRESULT ManagementFX::initDefaultVS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"defaultVS.cso";
	defaultVS_ = new ShaderVS();
	hr = defaultVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultPS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"defaultPS.cso";
	defaultPS_ = new ShaderPS();
	hr = defaultPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultDeferredVS(ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"defaultDeferredVS.cso";
	defaultDeferredVS_ = new ShaderVS();
	hr = defaultDeferredVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultDeferredPS(ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"defaultDeferredPS.cso";
	defaultDeferredPS_ = new ShaderPS();
	hr = defaultDeferredPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultCS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"lightingCS.cso";
	defaultCS_ = new ShaderCS();
	hr = defaultCS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initAnimationVS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"animationVS.cso";
	animationVS_ = new ShaderVS();
	hr = animationVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initAnimationPS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"animationPS.cso";
	animationPS_ = new ShaderPS();
	hr = animationPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initColorVS(ID3D11Device* device,				std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"colorVS.cso";
	colorVS_ = new ShaderVS();
	hr = colorVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initColorPS(ID3D11Device* device,				std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"colorPS.cso";
	colorPS_ = new ShaderPS();
	hr = colorPS_->init(device, completePath.c_str());

	return hr;
}

HRESULT ManagementFX::initILs(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	initILManagement();

	hr = initILPosColor(device);
	if(SUCCEEDED(hr))
		hr = initILDefaultVSPosNormTex(device);
	if(SUCCEEDED(hr))
		hr = initILPosNormTexTanSkinned(device);
	
	return hr;
}
void ManagementFX::initILManagement()
{
	managementIED_ = new ManagementIED();
	managementIED_->init();
}
HRESULT ManagementFX::initILPosColor(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[6] = 
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	device->CreateInputLayout(ied,
							  2,
							  colorVS_->getBlob()->GetBufferPointer(),
							  colorVS_->getBlob()->GetBufferSize(),
							  &ilPosColor_);
	return hr;
}
HRESULT ManagementFX::initILDefaultVSPosNormTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	unsigned int iedPosNormTexNumElements	= managementIED_->getIEDPosNormTexNumElements();
	D3D11_INPUT_ELEMENT_DESC* iedPosNormTex = managementIED_->getIEDPosNormTex();

	int debug = sizeof(D3D11_INPUT_ELEMENT_DESC);

	hr = device->CreateInputLayout(
		iedPosNormTex, 
		iedPosNormTexNumElements, 
		defaultVS_->getBlob()->GetBufferPointer(), 
		defaultVS_->getBlob()->GetBufferSize(), 
		&ilPosNormTex_);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initILDefaultVSPosNormTex CreateInputLayout failed");

	return hr;
}
HRESULT ManagementFX::initILPosNormTexTanSkinned(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ied[6] = 
	{
		{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	device->CreateInputLayout(ied,
							  6,
							  animationVS_->getBlob()->GetBufferPointer(),
							  animationVS_->getBlob()->GetBufferSize(),
							  &ilPosNormTexTanSkinned_);
	return hr;
}

Shader* ManagementFX::getShaderFromID(ShaderID shaderID)
{
	Shader* shader = nullptr;
	switch(shaderID)
	{
	case SHADERID_VS_DEFAULT:
		shader = defaultVS_;
		break;
	case SHADERID_PS_DEFAULT:
		shader = defaultPS_;
		break;
	case SHADERID_VS_DEFERRED_DEFAULT:
		shader = defaultDeferredVS_;
		break;
	case SHADERID_PS_DEFERRED_DEFAULT:
		shader = defaultDeferredPS_;
		break;
	case SHADERID_CS_DEFAULT:
		shader = defaultCS_;
		break;
	case SHADERID_VS_ANIMATION:
		shader = animationVS_;
		break;
	case SHADERID_PS_ANIMATION:
		shader = animationPS_;
		break;
	case SHADERID_VS_COLOR:
		shader = colorVS_;
		break;
	case SHADERID_PS_COLOR:
		shader = colorPS_;
		break;
	}

	return shader;
}