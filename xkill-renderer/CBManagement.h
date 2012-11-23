#ifndef XKILL_RENDERER_CBMANAGEMENT_H
#define XKILL_RENDERER_CBMANAGEMENT_H

#include <d3d11.h>

#include "D3DInterface.h"

#include "CBPerFrame.h"
#include "CBPerInstance.h"

//! Class for a constant buffer that will be updated every frame.
/*!
\ingroup xkill-renderer-constant-buffer
*/
class CBManagement
{
public:
	//! Sets CBManagement to its default state.
	CBManagement();
	//! Releases all memory and returns CBManagement to default state
	~CBManagement();
	//! Releases all memory and returns CBManagement to default state
	virtual void reset();
	//! Initializes CBManagement
	/*!
	\param device DirectX device pointer
	\return Any error that was encountered during initialization.
	*/
	HRESULT init(ID3D11Device* device);

	CBPerFrame*		getCBPerFrame()		const;
	CBPerInstance*	getCBPerInstance()	const;

private:
	//! Initializes cbPerFrame
	/*!
	\param device DirectX device pointer
	\return Any error that was encountered during initialization.
	*/
	HRESULT initCBPerFrame(ID3D11Device* device);
	//! Initializes cbPerInstance
	/*!
	\param device DirectX device pointer
	\return Any error that was encountered during initialization.
	*/
	HRESULT initCBPerInstance(ID3D11Device* device);

	CBPerFrame*		cbPerFrame_;	//!< Object containing a constant buffer that will be updated every frame.
	CBPerInstance*	cbPerInstance_;	//!< Object containing a constant buffer that will be update on initialization of the application.
};

#endif //XKILL_RENDERER_CBMANAGEMENT_H