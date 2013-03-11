#include <d3dcompiler.h>

#include "renderingUtilities.h"
#include "ManagementFX.h"

ManagementFX::ManagementFX(bool debugShaders)
{
	debugShaders_ = debugShaders;

	managementIED_ = nullptr;

	vsPosNormTexInstanced_		= nullptr;
	vsAnimation_				= nullptr;
	vsColor_					= nullptr;
	vsSprite_					= nullptr;
	vsPosNormTexTanInstanced_	= nullptr;
	vsScreenQuad_				= nullptr;

	psDefault_				= nullptr;
	psAnimation_			= nullptr;
	psColor_				= nullptr;
	psSprite_				= nullptr;
	psNormalMap_			= nullptr;
	psDownSample_			= nullptr;
	psBuildShadowMapPosNormTex_	= nullptr;

	csLighting_	= nullptr;
	csBlurHorz_ = nullptr;
	csBlurVert_ = nullptr;
	csBlurBilateralHorz_ = nullptr;
	csBlurBilateralVert_ = nullptr;
	csSSAO_		= nullptr;

	ilPosColor_				= nullptr;
	ilPosNormTexInstanced_	= nullptr;
	ilPosNormTexTanSkinned_ = nullptr;
	ilPosNormTex_			= nullptr;
	ilPosNormTexTanInstanced_ = nullptr;
}
ManagementFX::~ManagementFX()
{
	SAFE_DELETE(managementIED_);

	SAFE_DELETE(vsPosNormTexInstanced_);
	SAFE_DELETE(vsAnimation_);
	SAFE_DELETE(vsColor_);
	SAFE_DELETE(vsSprite_);
	SAFE_DELETE(vsPosNormTexTanInstanced_);
	SAFE_DELETE(vsScreenQuad_);
	
	SAFE_DELETE(psDefault_);
	SAFE_DELETE(psAnimation_);
	SAFE_DELETE(psColor_);
	SAFE_DELETE(psSprite_);
	SAFE_DELETE(psNormalMap_);
	SAFE_DELETE(psDownSample_);
	SAFE_DELETE(psBuildShadowMapPosNormTex_);
	
	SAFE_DELETE(csLighting_);
	SAFE_DELETE(csBlurHorz_);
	SAFE_DELETE(csBlurVert_);
	SAFE_DELETE(csBlurBilateralHorz_);
	SAFE_DELETE(csBlurBilateralVert_);
	SAFE_DELETE(csSSAO_);

	SAFE_RELEASE(ilPosColor_);
	SAFE_RELEASE(ilPosNormTexInstanced_);
	SAFE_RELEASE(ilPosNormTexTanSkinned_);
	SAFE_RELEASE(ilPosNormTex_);
	SAFE_RELEASE(ilPosNormTexTanInstanced_);
}

void ManagementFX::reset()
{
	vsPosNormTexInstanced_		->reset();
	vsAnimation_				->reset();
	vsColor_					->reset();
	vsSprite_					->reset();
	vsPosNormTexTanInstanced_	->reset();
	vsScreenQuad_				->reset();

	psDefault_					->reset();
	psAnimation_				->reset();
	psColor_					->reset();
	psSprite_					->reset();
	psNormalMap_				->reset();
	psDownSample_				->reset();
	psBuildShadowMapPosNormTex_	->reset();

	csLighting_	->reset();
	csBlurHorz_	->reset();
	csBlurVert_	->reset();
	csBlurBilateralHorz_->reset();
	csBlurBilateralVert_->reset();
	csSSAO_		->reset();
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

	hr = initVSPosNormTexInstanced(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initVSAnimation(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initVSColor(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initVSSprite(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initVSPosNormTexTanInstanced(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initVSScreenQuad(device, shaderPath);

	if(SUCCEEDED(hr))
		hr = initPSDefault(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSAnimation(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSColor(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSSprite(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSNormalMap(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSDownSample(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initPSBuildShadowMapPosNormTex(device, shaderPath);

	if(SUCCEEDED(hr))
		hr = initCSLighting(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initCSBlurHorz(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initCSBlurVert(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initCSBlurBilateralHorz(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initCSBlurBilateralVert(device, shaderPath);
	if(SUCCEEDED(hr))
		hr = initCSSSAO(device, shaderPath);
	
	return hr;
}
HRESULT ManagementFX::initVSPosNormTexInstanced(				ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_PosNormTexInstanced.cso";
	vsPosNormTexInstanced_ = new ShaderVS();
	hr = vsPosNormTexInstanced_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initVSAnimation(							ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_Animation.cso";
	vsAnimation_ = new ShaderVS();
	hr = vsAnimation_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initVSColor(								ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_Color.cso";
	vsColor_ = new ShaderVS();
	hr = vsColor_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initVSSprite(								ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_Sprite.cso";
	vsSprite_ = new ShaderVS();
	hr = vsSprite_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initVSPosNormTexTanInstanced(				ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"VS_PosNormTexTanInstanced.cso";
	vsPosNormTexTanInstanced_ = new ShaderVS();
	hr = vsPosNormTexTanInstanced_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initVSScreenQuad(							ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;

	std::wstring completePath = shaderPath + L"VS_ScreenQuad.cso";
	vsScreenQuad_ = new ShaderVS();
	hr = vsScreenQuad_->init(device, completePath.c_str());

	return hr;
}

HRESULT ManagementFX::initPSDefault(					ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Default.cso";
	psDefault_ = new ShaderPS();
	hr = psDefault_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSAnimation(					ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Animation.cso";
	psAnimation_ = new ShaderPS();
	hr = psAnimation_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSColor(						ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Color.cso";
	psColor_ = new ShaderPS();
	hr = psColor_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSSprite(						ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_Sprite.cso";
	psSprite_ = new ShaderPS();
	hr = psSprite_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSNormalMap(					ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"PS_NormalMap.cso";
	psNormalMap_ = new ShaderPS();
	hr = psNormalMap_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSDownSample(					ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	
	std::wstring completePath = shaderPath + L"PS_DownSample.cso";
	psDownSample_ = new ShaderPS();
	hr = psDownSample_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initPSBuildShadowMapPosNormTex(	ID3D11Device* device,	std::wstring shaderPath)
{
	HRESULT hr = S_OK;

	std::wstring completePath = shaderPath + L"PS_BuildShadowMapPosNormTex.cso";
	psBuildShadowMapPosNormTex_ = new ShaderPS();
	hr = psBuildShadowMapPosNormTex_->init(device, completePath.c_str());

	return hr;
}

HRESULT ManagementFX::initCSLighting(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"CS_Lighting.cso";
	csLighting_ = new ShaderCS();
	hr = csLighting_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initCSBlurHorz(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"CS_Blur_Horz.cso";
	csBlurHorz_ = new ShaderCS();
	hr = csBlurHorz_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initCSBlurVert(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"CS_Blur_Vert.cso";
	csBlurVert_ = new ShaderCS();
	hr = csBlurVert_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initCSBlurBilateralHorz(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"CS_Blur_Bilateral_Horz.cso";
	csBlurBilateralHorz_ = new ShaderCS();
	hr = csBlurBilateralHorz_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initCSBlurBilateralVert(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;
	std::wstring completePath = shaderPath + L"CS_Blur_Bilateral_Vert.cso";
	csBlurBilateralVert_ = new ShaderCS();
	hr = csBlurBilateralVert_->init(device, completePath.c_str());

	return hr;
}
HRESULT ManagementFX::initCSSSAO(ID3D11Device* device, std::wstring shaderPath)
{
	HRESULT hr = S_OK;

	std::wstring completePath = shaderPath + L"CS_SSAO.cso";
	csSSAO_ = new ShaderCS();
	hr = csSSAO_->init(device, completePath.c_str());

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
		vsColor_->getBlob()->GetBufferPointer(),
		vsColor_->getBlob()->GetBufferSize(),
		&ilPosColor_);

	return hr;
}
HRESULT ManagementFX::initILPosNormTexInstanced(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = device->CreateInputLayout(
		managementIED_->getIED(IED_TYPE__POS_NORM_TEX_INSTANCED), 
		managementIED_->getIEDNumElements(IED_TYPE__POS_NORM_TEX_INSTANCED), 
		vsPosNormTexInstanced_->getBlob()->GetBufferPointer(), 
		vsPosNormTexInstanced_->getBlob()->GetBufferSize(), 
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
	//	{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"BONEINDICES",  0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};


	hr = device->CreateInputLayout(managementIED_->getIED(IED_TYPE__POS_NORM_TEX_TAN_SKINNED),
							  managementIED_->getIEDNumElements(IED_TYPE__POS_NORM_TEX_TAN_SKINNED),
							  vsAnimation_->getBlob()->GetBufferPointer(),
							  vsAnimation_->getBlob()->GetBufferSize(),
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
		vsSprite_->getBlob()->GetBufferPointer(),
		vsSprite_->getBlob()->GetBufferSize(),
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
	case SHADERID_VS_POS_NORM_TEX_INSTANCE:
		shader = vsPosNormTexInstanced_;
		break;
	case SHADERID_VS_ANIMATION:
		shader = vsAnimation_;
		break;
	case SHADERID_VS_COLOR:
		shader = vsColor_;
		break;
	case SHADERID_VS_SPRITE:
		shader = vsSprite_;
		break;
	case SHADERID_VS_POS_NORM_TEX_TAN_INSTANCE:
		shader = vsPosNormTexTanInstanced_;
		break;
	case SHADERID_VS_SCREENQUAD:
		shader = vsScreenQuad_;
		break;

	case SHADERID_PS_DEFAULT:
		shader = psDefault_;
		break;
	case SHADERID_PS_ANIMATION:
		shader = psAnimation_;
		break;
	case SHADERID_PS_COLOR:
		shader = psColor_;
		break;
	case SHADERID_PS_SPRITE:
		shader = psSprite_;
		break;
	case SHADERID_PS_NORMALMAP:
		shader = psNormalMap_;
		break;
	case SHADERID_PS_DOWNSAMPLE:
		shader = psDownSample_;
		break;
	case SHADERID_PS_BUILD_SHADOWMAP_POS_NORM_TEX:
		shader = psBuildShadowMapPosNormTex_;
		break;

	case SHADERID_CS_LIGHTING:
		shader = csLighting_;
		break;
	case SHADERID_CS_BLUR_HORZ:
		shader = csBlurHorz_;
		break;
	case SHADERID_CS_BLUR_VERT:
		shader = csBlurVert_;
		break;
	case SHADERID_CS_BLUR_BILATERAL_HORZ:
		shader = csBlurBilateralHorz_;
		break;
	case SHADERID_CS_BLUR_BILATERAL_VERT:
		shader = csBlurBilateralVert_;
		break;
	case SHADERID_CS_SSAO:
		shader = csSSAO_;
		break;
	}

	return shader;
}