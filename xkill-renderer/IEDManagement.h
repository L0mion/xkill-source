#ifndef XKILL_RENDERER_ILMANAGEMENT_H
#define XKILL_RENDERER_ILMANAGEMENT_H

#include <d3d11.h>

class IEDManagement
{
public:
	IEDManagement();
	virtual ~IEDManagement();

	void init();

	unsigned int getIEDPosNormTexNumElements();
	D3D11_INPUT_ELEMENT_DESC* getIEDPosNormTex();
protected:
private:
	void initIEDPosNormTex();

	D3D11_INPUT_ELEMENT_DESC createIED(
		LPCSTR		semanticName,
		DXGI_FORMAT	format);
	D3D11_INPUT_ELEMENT_DESC IED(
		LPCSTR						semanticName,
		unsigned int				semanticIndex,
		DXGI_FORMAT					format,
		unsigned int				inputSlot,
		unsigned int				alignedByteOffset,
		D3D11_INPUT_CLASSIFICATION	inputSlotClass,
		unsigned int				instanceDataStepRate);

	/*Fugly statics*/
	static const unsigned int defaultSemanticIndex_					= 0;
	static const unsigned int defaultInputSlot_						= 0;
	static const unsigned int defaultAlignedByteOffset_				= D3D11_APPEND_ALIGNED_ELEMENT;
	static const D3D11_INPUT_CLASSIFICATION defaultInputSlotClass_	= D3D11_INPUT_PER_VERTEX_DATA;
	static const unsigned int defaultInstanceDataStepRate_			= 0;

	/*Semantic descriptors*/
	const LPCSTR semanticPosition_;
	const LPCSTR semanticNormal_;
	const LPCSTR semanticTexcoord_;

	static const unsigned int iedPosNormTexNumElements_ = 3;
	D3D11_INPUT_ELEMENT_DESC iedPosNormTex_[iedPosNormTexNumElements_];
};

#endif //XKILL_RENDERER_ILMANAGEMENT_H