#ifndef XKILL_RENDERER_ILMANAGEMENT_H
#define XKILL_RENDERER_ILMANAGEMENT_H

#include <d3d11.h>

//! Stores predefined types of Input Element descriptions required to initialize Input Layouts.
/*!
\ingroup xkill-renderer
\sa D3D11_INPUT_ELEMENT_DESC
*/
class ManagementIED
{
public:
	//! Sets IEDManagement to default state.
	/*!
	Nulls memory of member IEDs.
	\sa iedPosNormTex_
	*/
	ManagementIED();

	//! Does nothing.
	virtual ~ManagementIED();

	//! Initializes member IEDs.
	/*!
	\sa initIEDPosNormTex
	*/
	void init();
	
	unsigned int				getIEDPosNormTexNumElements();
	D3D11_INPUT_ELEMENT_DESC*	getIEDPosNormTex();
protected:
private:
	//! Initializes IED consisting of Position-, Normal- and TexCoord-attributes.
	/*!
	\sa createIED
	*/
	void initIEDPosNormTex();

	//! Method reducing complexity of standard IED-constructor.
	/*!
	\param semanticName		Semantic of attribute.
	\param format			Format of attribute.
	\sa IED
	\sa defaultSemanticIndex_
	\sa defaultInputSlot_
	\sa defaultAlignedByteOffset_
	\sa defaultInputSlotClass_
	\sa defaultInstanceDataStepRate_
	\sa DXGI_FORMAT
	*/
	D3D11_INPUT_ELEMENT_DESC createIED(
		LPCSTR		semanticName,
		DXGI_FORMAT	format);
	//! Standard IED-constructor.
	/*!
	http://msdn.microsoft.com/en-us/library/windows/desktop/ff476180%28v=vs.85%29.aspx
	\param semanticName			The HLSL semantic associated with this element in a shader input-signature.
	\param semanticIndex		The semantic index for the element. A semantic index modifies a semantic, with an integer index number. A semantic index is only needed in a case where there is more than one element with the same semantic. For example, a 4x4 matrix would have four components each with the semantic name 
	\param format				The data type of the element data.
	\param inputSlot			An integer value that identifies the input-assembler (see input slot). Valid values are between 0 and 15, defined in D3D11.h.
	\param alignedByteOffset	Optional. Offset (in bytes) between each element. Use D3D11_APPEND_ALIGNED_ELEMENT for convenience to define the current element directly after the previous one, including any packing if necessary.
	\param inputSlotClass		Identifies the input data class for a single input slot.
	\param instanceDataStepRate	The number of instances to draw using the same per-instance data before advancing in the buffer by one element. This value must be 0 for an element that contains per-vertex data (the slot class is set to D3D11_INPUT_PER_VERTEX_DATA).
	\sa D3D11_INPUT_ELEMENT_DESC
	\sa DXGI_FORMAT
	\sa D3D11_INPUT_CLASSIFICATION
	*/
	D3D11_INPUT_ELEMENT_DESC IED(
		LPCSTR						semanticName,
		unsigned int				semanticIndex,
		DXGI_FORMAT					format,
		unsigned int				inputSlot,
		unsigned int				alignedByteOffset,
		D3D11_INPUT_CLASSIFICATION	inputSlotClass,
		unsigned int				instanceDataStepRate);

	//! The semantic index for the element. A semantic index modifies a semantic, with an integer index number. A semantic index is only needed in a case where there is more than one element with the same semantic. For example, a 4x4 matrix would have four components each with the semantic name 
	static const unsigned int defaultSemanticIndex_					= 0;
	//! An integer value that identifies the input-assembler (see input slot). Valid values are between 0 and 15, defined in D3D11.h.
	static const unsigned int defaultInputSlot_						= 0;
	//! Offset (in bytes) between each element. Use D3D11_APPEND_ALIGNED_ELEMENT for convenience to define the current element directly after the previous one, including any packing if necessary.
	static const unsigned int defaultAlignedByteOffset_				= D3D11_APPEND_ALIGNED_ELEMENT;
	//! Identifies the input data class for a single input slot.
	static const D3D11_INPUT_CLASSIFICATION defaultInputSlotClass_	= D3D11_INPUT_PER_VERTEX_DATA;
	//! The number of instances to draw using the same per-instance data before advancing in the buffer by one element. This value must be 0 for an element that contains per-vertex data (the slot class is set to D3D11_INPUT_PER_VERTEX_DATA).
	static const unsigned int defaultInstanceDataStepRate_			= 0;

	/*Semantic descriptors*/
	const LPCSTR semanticPosition_;	//!< Defined semantic of POSITION in D3D.
	const LPCSTR semanticNormal_;	//!< Defined semantic of NORMAL in D3D.
	const LPCSTR semanticTexcoord_;	//!< Defined semantic of TEXCOORD in D3D.

	//! Number of elements in PosNormTex-IED.
	static const unsigned int iedPosNormTexNumElements_ = 3;
	//! IED describing PosNormTex-Input-Layout.
	D3D11_INPUT_ELEMENT_DESC iedPosNormTex_[iedPosNormTexNumElements_];
};

#endif //XKILL_RENDERER_ILMANAGEMENT_H