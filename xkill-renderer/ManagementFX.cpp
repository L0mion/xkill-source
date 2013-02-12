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
	spriteVS_				= nullptr;
	spritePS_				= nullptr;
	vsPosNormTexTanInstanced_ = nullptr;
	psNormalMap_				= nullptr;

	ilPosColor_				= nullptr;
	ilPosNormTexInstanced_	= nullptr;
	ilPosNormTexTanSkinned_ = nullptr;
	ilPosNormTex_			= nullptr;
	ilPosNormTexTanInstanced_ = nullptr;
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
	SAFE_DELETE(spriteVS_);
	SAFE_DELETE(spritePS_);
	SAFE_DELETE(vsPosNormTexTanInstanced_);
	SAFE_DELETE(psNormalMap_);

	SAFE_RELEASE(ilPosColor_);
	SAFE_RELEASE(ilPosNormTexInstanced_);
	SAFE_RELEASE(ilPosNormTexTanSkinned_);
	SAFE_RELEASE(ilPosNormTex_);
	SAFE_RELEASE(ilPosNormTexTanInstanced_);
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
	vsPosNormTexTanInstanced_->reset();
	psNormalMap_->reset();
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
	case LAYOUTID_POS_COLOR:
		il = ilPosColor_;
		break;
	case LAYOUTID_POS_NORM_TEX_INSTANCED:
		il = ilPosNormTexInstanced_;
		break;
	case LAYOUTID_POS_NORM_TEX_TAN_SKINNED:
		il = ilPosNormTexTanSkinned_;
		break;
	case LAYOUTID_POS_NORM_TEX:
		il = ilPosNormTex_;
		break;
	case LAYOUTID_POS_NORM_TEX_TAN_INSTANCED:
		il = ilPosNormTexTanInstanced_;
		break;
	};

	if(il)
		devcon->IASetInputLayout(il);
}
void ManagementFX::unsetLayout(ID3D11DeviceContext* devcon)
{
	devcon->IASetInputLayout(nullptr);
}

void ManagementFX::unsetAll(ID3D11DeviceContext* devcon)
{
	devcon->VSSetShader(NULL, NULL, 0);
	devcon->GSSetShader(NULL, NULL, 0);
	devcon->HSSetShader(NULL, NULL, 0);
	devcon->DSSetShader(NULL, NULL, 0);
	devcon->PSSetShader(NULL, NULL, 0);
	devcon->CSSetShader(NULL, NULL, 0);
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
	if(SUCCEEDED(hr))
		hr = initSpriteVS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initSpritePS(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initVSPosNormTexTanInstanced(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSNormalMap(device, shaderPath);
	
	return hr;
}
HRESULT ManagementFX::initDefaultVS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_PosNormTexInstanced.cso";
	defaultVS_ = new ShaderVS();
	hr = defaultVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultPS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Default.cso";
	defaultPS_ = new ShaderPS();
	hr = defaultPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultDeferredVS(ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_DefaultDeferred.cso";
	defaultDeferredVS_ = new ShaderVS();
	hr = defaultDeferredVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultDeferredPS(ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_DefaultDeferred.cso";
	defaultDeferredPS_ = new ShaderPS();
	hr = defaultDeferredPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initDefaultCS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"CS_Lighting.cso";
	defaultCS_ = new ShaderCS();
	hr = defaultCS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initAnimationVS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_Animation.cso";
	animationVS_ = new ShaderVS();
	hr = animationVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initAnimationPS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Animation.cso";
	animationPS_ = new ShaderPS();
	hr = animationPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initColorVS(ID3D11Device* device,				std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_Color.cso";
	colorVS_ = new ShaderVS();
	hr = colorVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initColorPS(ID3D11Device* device,				std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Color.cso";
	colorPS_ = new ShaderPS();
	hr = colorPS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initSpriteVS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_Sprite.cso";
	spriteVS_ = new ShaderVS();
	hr = spriteVS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initSpritePS(ID3D11Device* device,			std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Sprite.cso";
	spritePS_ = new ShaderPS();
	hr = spritePS_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initVSPosNormTexTanInstanced(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_PosNormTexTanInstanced.cso";
	vsPosNormTexTanInstanced_ = new ShaderVS();
	hr = vsPosNormTexTanInstanced_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSNormalMap(ID3D11Device*	device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_NormalMap.cso";
	psNormalMap_ = new ShaderPS();
	hr = psNormalMap_->init(device, completePath.c_str());

	return hr;
}

HRESULT ManagementFX::initILs(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	initILManagement();

	hr = initILPosColor(device);
	if(SUCCEEDED(hr))
		hr = initILPosNormTexInstanced(device);
	if(SUCCEEDED(hr))
		hr = initILPosNormTexTanSkinned(device);
	if(SUCCEEDED(hr))
		hr = initILPosNormTex(device);
	if(SUCCEEDED(hr))
		hr = initILPosNormtexTanInstanced(device);

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

	device->CreateInputLayout(
		managementIED_->getIED(IED_TYPE__POS_COLOR),
		managementIED_->getIEDNumElements(IED_TYPE__POS_COLOR),
		colorVS_->getBlob()->GetBufferPointer(),
		colorVS_->getBlob()->GetBufferSize(),
		&ilPosColor_);

	return hr;
}
HRESULT ManagementFX::initILPosNormTexInstanced(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = device->CreateInputLayout(
		managementIED_->getIED(IED_TYPE__POS_NORM_TEX_INSTANCED), 
		managementIED_->getIEDNumElements(IED_TYPE__POS_NORM_TEX_INSTANCED), 
		defaultVS_->getBlob()->GetBufferPointer(), 
		defaultVS_->getBlob()->GetBufferSize(), 
		&ilPosNormTexInstanced_);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initILDefaultVSPosNormTexInstanced CreateInputLayout failed");

	return hr;
}
HRESULT ManagementFX::initILPosNormTexTanSkinned(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	//D3D11_INPUT_ELEMENT_DESC ied[6] = 
	//{
	//	{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};

	hr = device->CreateInputLayout(managementIED_->getIED(IED_TYPE__POS_NORM_TEX_TAN_SKINNED),
							  managementIED_->getIEDNumElements(IED_TYPE__POS_NORM_TEX_TAN_SKINNED),
							  animationVS_->getBlob()->GetBufferPointer(),
							  animationVS_->getBlob()->GetBufferSize(),
							  &ilPosNormTexTanSkinned_);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initILPosNormTexTanSkinned CreateInputLayout failed");

	return hr;
}
HRESULT ManagementFX::initILPosNormTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = device->CreateInputLayout(
		managementIED_->getIED(IED_TYPE__POS_NORM_TEX),
		managementIED_->getIEDNumElements(IED_TYPE__POS_NORM_TEX),
		spriteVS_->getBlob()->GetBufferPointer(),
		spriteVS_->getBlob()->GetBufferSize(),
		&ilPosNormTex_);

	return hr;
}
HRESULT ManagementFX::initILPosNormtexTanInstanced(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = device->CreateInputLayout(
		managementIED_->getIED(IED_TYPE__POS_NORM_TEX_TAN_INSTANCED), 
		managementIED_->getIEDNumElements(IED_TYPE__POS_NORM_TEX_TAN_INSTANCED), 
		vsPosNormTexTanInstanced_->getBlob()->GetBufferPointer(), 
		vsPosNormTexTanInstanced_->getBlob()->GetBufferSize(), 
		&ilPosNormTexTanInstanced_);
	if(FAILED(hr))
		ERROR_MSG(L"FXManagement::initILPosNormtexTanInstanced CreateInputLayout failed");

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
	case SHADERID_VS_SPRITE:
		shader = spriteVS_;
		break;
	case SHADERID_PS_SPRITE:
		shader = spritePS_;
		break;
	case SHADERID_VS_POS_NORM_TEX_TAN_INSTANCE:
		shader = vsPosNormTexTanInstanced_;
		break;
	case SHADERID_PS_NORMALMAP:
		shader = psNormalMap_;
		break;
	}

	return shader;
}