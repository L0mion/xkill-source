#ifndef XKILL_RENDERER_ILMANAGEMENT_H
#define XKILL_RENDERER_ILMANAGEMENT_H

#include <d3d11.h>

enum IED_TYPE
{
	IED_TYPE__POS_NORM_TEX,
	IED_TYPE__POS_COLOR,
	IED_TYPE__POS_NORM_TEX_TAN_SKINNED
};

//! Stores predefined types of Input Element descriptions required to initialize Input Layouts.
/*!
\ingroup xkill-renderer
*/
class ManagementIED
{
public:
	ManagementIED();			//!< Sets IEDManagement to default state.
	virtual ~ManagementIED();	//!< Does nothing.
	
	void init(); //!< Initializes member IEDs.

	unsigned int getIEDNumElements(IED_TYPE iedType);
	D3D11_INPUT_ELEMENT_DESC* getIED(IED_TYPE iedType);
protected:
private:
	void initIEDPosNormTex();			//!< Initializes IED consisting of Position-, Normal- and TexCoord-attributes.
	void initIEDPosColor();				//!< Initializes IED consisting of Position- and Color-attributes.
	void initIEDPosNormTexTanSkinned();	//!< Initializes IED consisting of Position-, Normal-, TexCoord-, Tangent-, Weights- and BoneIndices-attributes.

	D3D11_INPUT_ELEMENT_DESC createIED(
		LPCSTR		semanticName,
		DXGI_FORMAT	format); //!< Method reducing complexity of standard IED-constructor.
	D3D11_INPUT_ELEMENT_DESC IED(
		LPCSTR						semanticName,
		unsigned int				semanticIndex,
		DXGI_FORMAT					format,
		unsigned int				inputSlot,
		unsigned int				alignedByteOffset,
		D3D11_INPUT_CLASSIFICATION	inputSlotClass,
		unsigned int				instanceDataStepRate); //!< Standard IED-constructor.

	static const unsigned int				defaultSemanticIndex_			= 0;							//!< The semantic index for the element. A semantic index modifies a semantic, with an integer index number. A semantic index is only needed in a case where there is more than one element with the same semantic. For example, a 4x4 matrix would have four components each with the semantic name 
	static const unsigned int				defaultInputSlot_				= 0;							//!< An integer value that identifies the input-assembler (see input slot). Valid values are between 0 and 15, defined in D3D11.h.
	static const unsigned int				defaultAlignedByteOffset_		= D3D11_APPEND_ALIGNED_ELEMENT;	//!< Offset (in bytes) between each element. Use D3D11_APPEND_ALIGNED_ELEMENT for convenience to define the current element directly after the previous one, including any packing if necessary.
	static const D3D11_INPUT_CLASSIFICATION	defaultInputSlotClass_			= D3D11_INPUT_PER_VERTEX_DATA;	//!< Identifies the input data class for a single input slot.
	static const unsigned int				defaultInstanceDataStepRate_	= 0;							//!< The number of instances to draw using the same per-instance data before advancing in the buffer by one element. This value must be 0 for an element that contains per-vertex data (the slot class is set to D3D11_INPUT_PER_VERTEX_DATA).

	/*Semantic descriptors*/
	const LPCSTR semanticPosition_;		//!< Defined semantic of POSITION in D3D.
	const LPCSTR semanticNormal_;		//!< Defined semantic of NORMAL in D3D.
	const LPCSTR semanticTexcoord_;		//!< Defined semantic of TEXCOORD in D3D.
	const LPCSTR semanticColor_;		//!< Defined semantic of COLOR in D3D.
	const LPCSTR semanticTangent_;		//!< Defined semantic of TANGENT in D3D.
	const LPCSTR semanticWeights_;		//!< Defined semantic of WEIGHTS in D3D.
	const LPCSTR semanticBoneIndices_;	//!< Defined semantic of BONEINDICES in D3D.

	static const unsigned int iedPosNormTexNumElements_ = 3;			//!< Number of elements in PosNormTex-IED.
	D3D11_INPUT_ELEMENT_DESC iedPosNormTex_[iedPosNormTexNumElements_];	//!< IED describing PosNormTex-Input-Layout.

	static const unsigned int iedPosColorNumElements_ = 2;				//!< Number of elements in PosColor-IED.
	D3D11_INPUT_ELEMENT_DESC iedPosColor_[iedPosColorNumElements_];		//!< IED describing PosColor-Input-Layout.

	static const unsigned int iedPosNormTexTanSkinnedNumElements_ = 6;
	D3D11_INPUT_ELEMENT_DESC iedPosNormTexTanSkinned_[iedPosNormTexTanSkinnedNumElements_];
};

#endif //XKILL_RENDERER_ILMANAGEMENT_H