#include <ILManagement.h>

ILManagement::ILManagement() :
	semanticPosition_("POSITION"),
	semanticNormal_("NORMAL"),
	semanticTexcoord_ ("TEXCOORD")
{
	ZeroMemory(
		iedPosNormTex_, 
		sizeof(D3D11_INPUT_ELEMENT_DESC) * iedPosNormTexNumElements_);
}
ILManagement::~ILManagement()
{
}

void ILManagement::init()
{
	initIEDPosNormTex();
}

void ILManagement::initIEDPosNormTex()
{
	iedPosNormTex_[0] = createIED(semanticPosition_,	DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTex_[1] = createIED(semanticNormal_,		DXGI_FORMAT_R32G32B32_FLOAT);
	iedPosNormTex_[2] = createIED(semanticTexcoord_,	DXGI_FORMAT_R32G32_FLOAT);
}

D3D11_INPUT_ELEMENT_DESC ILManagement::createIED(
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
D3D11_INPUT_ELEMENT_DESC ILManagement::IED(
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

unsigned int ILManagement::getIEDPosNormTexNumElements()
{
	return iedPosNormTexNumElements_;
}
D3D11_INPUT_ELEMENT_DESC* ILManagement::getIEDPosNormTex()
{
	return iedPosNormTex_;
}