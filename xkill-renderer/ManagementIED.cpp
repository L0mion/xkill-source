#include "ManagementIED.h"

ManagementIED::ManagementIED() :
	semanticPosition_		("POSITION"),
	semanticNormal_			("NORMAL"),
	semanticTexcoord_		("TEXCOORD"),
	semanticColor_			("COLOR"),
	semanticTangent_		("TANGENT"),
	semanticWeights_		("WEIGHTS"),
	semanticBoneIndices_	("BONEINDICES"),
	semanticWorldTransform_	("WORLD")
{
	ZeroMemory(
		iedPosNormTex_, 
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexNumElements_);
	ZeroMemory(
		iedPosNormTexInstanced_,
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexInstancedNumElements_);
	ZeroMemory(
		iedPosColor_,
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosColorNumElements_);
	ZeroMemory(
		iedPosNormTexTanSkinned_,
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexTanSkinnedNumElements_);
	ZeroMemory(
		iedPosNormTexTanInstanced_,
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexTanInstancedNumElements_);
}
ManagementIED::~ManagementIED()
{
	//Do nothing.
}

void ManagementIED::init()
{
	initIEDPosNormTex();
	initIEDPosNormTexInstanced();
	initIEDPosColor();
	initIEDPosNormTexTanSkinned();
	initIEDPosNormTexTanInstanced();
}
void ManagementIED::initIEDPosNormTex()
{
	unsigned int semanticIndex	= 0;
	unsigned int dataStepRate	= 0;

	iedPosNormTex_[0] = createIED(semanticPosition_,	DXGI_FORMAT_R32G32B32_FLOAT,	semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTex_[1] = createIED(semanticNormal_,		DXGI_FORMAT_R32G32B32_FLOAT,	semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTex_[2] = createIED(semanticTexcoord_,	DXGI_FORMAT_R32G32_FLOAT,		semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
}
void ManagementIED::initIEDPosNormTexInstanced()
{
	iedPosNormTexInstanced_[0] = createIED(semanticPosition_,		DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_INPUT_PER_VERTEX_DATA,		0);
	iedPosNormTexInstanced_[1] = createIED(semanticNormal_,			DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_INPUT_PER_VERTEX_DATA,		0);
	iedPosNormTexInstanced_[2] = createIED(semanticTexcoord_,		DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_INPUT_PER_VERTEX_DATA,		0);

	iedPosNormTexInstanced_[3] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_INSTANCE_DATA,	1);
	iedPosNormTexInstanced_[4] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_INPUT_PER_INSTANCE_DATA,	1);
	iedPosNormTexInstanced_[5] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_INPUT_PER_INSTANCE_DATA,	1);
	iedPosNormTexInstanced_[6] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_INPUT_PER_INSTANCE_DATA,	1);
}
void ManagementIED::initIEDPosColor()
{
	unsigned int semanticIndex	= 0;
	unsigned int dataStepRate	= 0;

	iedPosColor_[0] = createIED(semanticPosition_,	DXGI_FORMAT_R32G32B32_FLOAT, semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosColor_[1] = createIED(semanticColor_,		DXGI_FORMAT_R32G32B32_FLOAT, semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
}
void ManagementIED::initIEDPosNormTexTanSkinned()
{
	unsigned int semanticIndex	= 0;
	unsigned int dataStepRate	= 0;

	iedPosNormTexTanSkinned_[0] = createIED(semanticPosition_,		DXGI_FORMAT_R32G32B32_FLOAT,	semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTexTanSkinned_[1] = createIED(semanticNormal_,		DXGI_FORMAT_R32G32B32_FLOAT,	semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTexTanSkinned_[2] = createIED(semanticTexcoord_,		DXGI_FORMAT_R32G32_FLOAT,		semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTexTanSkinned_[3] = createIED(semanticTangent_,		DXGI_FORMAT_R32G32B32A32_FLOAT,	semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTexTanSkinned_[4] = createIED(semanticWeights_,		DXGI_FORMAT_R32G32B32_FLOAT,	semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
	iedPosNormTexTanSkinned_[5] = createIED(semanticBoneIndices_,	DXGI_FORMAT_R8G8B8A8_UINT,		semanticIndex, D3D11_INPUT_PER_VERTEX_DATA, dataStepRate);
}
void ManagementIED::initIEDPosNormTexTanInstanced()
{
	iedPosNormTexTanInstanced_[0] = createIED(semanticPosition_,		DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_INPUT_PER_VERTEX_DATA,		0);
	iedPosNormTexTanInstanced_[1] = createIED(semanticNormal_,			DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_INPUT_PER_VERTEX_DATA,		0);
	iedPosNormTexTanInstanced_[2] = createIED(semanticTexcoord_,		DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_INPUT_PER_VERTEX_DATA,		0);
	iedPosNormTexTanInstanced_[3] = createIED(semanticTangent_,			DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_INPUT_PER_VERTEX_DATA,		0);

	iedPosNormTexTanInstanced_[4] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_INSTANCE_DATA,	1);
	iedPosNormTexTanInstanced_[5] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_INPUT_PER_INSTANCE_DATA,	1);
	iedPosNormTexTanInstanced_[6] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_INPUT_PER_INSTANCE_DATA,	1);
	iedPosNormTexTanInstanced_[7] = createIED(semanticWorldTransform_,	DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_INPUT_PER_INSTANCE_DATA,	1);
}

unsigned int ManagementIED::getIEDNumElements(IED_TYPE iedType)
{
	unsigned int iedNumElements = 0;
	switch(iedType)
	{
	case IED_TYPE__POS_NORM_TEX:
		iedNumElements = iedPosNormTexNumElements_;
		break;
	case IED_TYPE__POS_NORM_TEX_INSTANCED:
		iedNumElements = iedPosNormTexInstancedNumElements_;
		break;
	case IED_TYPE__POS_COLOR:
		iedNumElements = iedPosColorNumElements_;
		break;
	case IED_TYPE__POS_NORM_TEX_TAN_SKINNED:
		iedNumElements = iedPosNormTexTanSkinnedNumElements_;
		break;
	case IED_TYPE__POS_NORM_TEX_TAN_INSTANCED:
		iedNumElements = iedPosNormTexTanInstancedNumElements_;
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
	case IED_TYPE__POS_NORM_TEX_INSTANCED:
		ied = iedPosNormTexInstanced_;
		break;
	case IED_TYPE__POS_COLOR:
		ied = iedPosColor_;
		break;
	case IED_TYPE__POS_NORM_TEX_TAN_SKINNED:
		ied = iedPosNormTexTanSkinned_;
		break;
	case IED_TYPE__POS_NORM_TEX_TAN_INSTANCED:
		ied = iedPosNormTexTanInstanced_;
		break;
	}

	return ied;
}

D3D11_INPUT_ELEMENT_DESC ManagementIED::createIED(
	LPCSTR						semanticName,
	DXGI_FORMAT					format,
	unsigned int				semanticIndex,
	D3D11_INPUT_CLASSIFICATION	inputSlotClass,
	unsigned int				instanceDataStepRate)
{
	unsigned int inputSlot = 0;
	switch(inputSlotClass)
	{
	case D3D11_INPUT_PER_VERTEX_DATA:
		inputSlot = 0;
		break;
	case D3D11_INPUT_PER_INSTANCE_DATA:
		inputSlot = 1;
		break;
	}

	D3D11_INPUT_ELEMENT_DESC ied
		= IED(
		semanticName,
		semanticIndex,
		format,
		inputSlot,
		defaultAlignedByteOffset_,
		inputSlotClass,
		instanceDataStepRate); 

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