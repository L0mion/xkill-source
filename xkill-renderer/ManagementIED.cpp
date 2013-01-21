#include "ManagementIED.h"

ManagementIED::ManagementIED() :
	semanticPosition_	("POSITION"),
	semanticNormal_		("NORMAL"),
	semanticTexcoord_	("TEXCOORD"),
	semanticColor_		("COLOR"),
	semanticTangent_	("TANGENT"),
	semanticWeights_	("WEIGHTS"),
	semanticBoneIndices_("BONEINDICES")
{
	ZeroMemory(
		iedPosNormTex_, 
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexNumElements_);
	ZeroMemory(
		iedPosColor_,
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosColorNumElements_);
	ZeroMemory(
		iedPosNormTexTanSkinned_,
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexTanSkinnedNumElements_);
}
ManagementIED::~ManagementIED()
{
	//Do nothing.
}

void ManagementIED::init()
{
	initIEDPosNormTex();
	initIEDPosColor();
	initIEDPosNormTexTanSkinned();
}
void ManagementIED::initIEDPosNormTex()
{
	iedPosNormTex_[0] = createIED(semanticPosition_,	DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTex_[1] = createIED(semanticNormal_,		DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTex_[2] = createIED(semanticTexcoord_,	DXGI_FORMAT_R32G32_FLOAT);
}
void ManagementIED::initIEDPosColor()
{
	iedPosColor_[0] = createIED(semanticPosition_,	DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosColor_[1] = createIED(semanticColor_,		DXGI_FORMAT_R32G32B32_FLOAT);
}
void ManagementIED::initIEDPosNormTexTanSkinned()
{
	iedPosNormTexTanSkinned_[0] = createIED(semanticPosition_,		DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTexTanSkinned_[1] = createIED(semanticNormal_,		DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTexTanSkinned_[2] = createIED(semanticTexcoord_,		DXGI_FORMAT_R32G32_FLOAT);
	iedPosNormTexTanSkinned_[3] = createIED(semanticTangent_,		DXGI_FORMAT_R32G32B32A32_FLOAT);
	iedPosNormTexTanSkinned_[4] = createIED(semanticWeights_,		DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTexTanSkinned_[5] = createIED(semanticBoneIndices_,	DXGI_FORMAT_R8G8B8A8_UINT);
}

unsigned int ManagementIED::getIEDNumElements(IED_TYPE iedType)
{
	unsigned int iedNumElements = 0;
	switch(iedType)
	{
	case IED_TYPE__POS_NORM_TEX:
		iedNumElements = iedPosNormTexNumElements_;
		break;
	case IED_TYPE__POS_COLOR:
		iedNumElements = iedPosColorNumElements_;
		break;
	case IED_TYPE__POS_NORM_TEX_TAN_SKINNED:
		iedNumElements = iedPosNormTexTanSkinnedNumElements_;
		break;
	}

	return iedNumElements;
}
D3D11_INPUT_ELEMENT_DESC* ManagementIED::getIED(IED_TYPE iedType)
{
	D3D11_INPUT_ELEMENT_DESC* ied = nullptr;
	switch(iedType)
	{
	case IED_TYPE__POS_NORM_TEX:
		ied = iedPosNormTex_;
		break;
	case IED_TYPE__POS_COLOR:
		ied = iedPosColor_;
		break;
	case IED_TYPE__POS_NORM_TEX_TAN_SKINNED:
		ied = iedPosNormTexTanSkinned_;
		break;
	}

	return ied;
}

D3D11_INPUT_ELEMENT_DESC ManagementIED::createIED(
	LPCSTR		semanticName,
	DXGI_FORMAT	format)
{
	D3D11_INPUT_ELEMENT_DESC ied
		= IED(
		semanticName,
		defaultSemanticIndex_,
		format,
		defaultInputSlot_,
		defaultAlignedByteOffset_,
		defaultInputSlotClass_,
		defaultInstanceDataStepRate_); 

	return ied;
}
D3D11_INPUT_ELEMENT_DESC ManagementIED::IED(
	LPCSTR						semanticName,
	unsigned int				semanticIndex,
	DXGI_FORMAT					format,
	unsigned int				inputSlot,
	unsigned int				alignedByteOffset,
	D3D11_INPUT_CLASSIFICATION	inputSlotClass,
	unsigned int				instanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC ied;
	ied.SemanticName			= semanticName;
	ied.SemanticIndex			= (UINT)semanticIndex;
	ied.Format					= format;
	ied.InputSlot				= (UINT)inputSlot;
	ied.AlignedByteOffset		= (UINT)alignedByteOffset;
	ied.InputSlotClass			= inputSlotClass;
	ied.InstanceDataStepRate	= (UINT)instanceDataStepRate;

	return ied;
}