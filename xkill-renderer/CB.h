#ifndef XKILL_RENDERER_CB_H
#define XKILL_RENDERER_CB_H

#include <d3d11.h>

#include "D3DInterface.h"

/*! \defgroup xkill-renderer-constant-buffer xkill-renderer-constant-buffer
	Package wrapping constant buffers in DirectX.
	\ingroup xkill-renderer
	*/

//! Base class for all types of constant buffers used by the renderer.
/*!
\ingroup xkill-renderer-constant-buffer
*/
class CB : public D3DInterface
{
public:
	//! Sets CB to default state.
	CB();
	//! Releases all memory and returns CB to default state
	virtual ~CB();
	//! Releases all memory and returns CB to default state
	virtual void reset();
	//! Initializes CB
	/*!
	\param device DirectX device pointer
	\param size The size of the constant buffer which has to be a multiple of 16 bytes.
	\return Any error that was encountered during initialization.
	*/
	HRESULT init(ID3D11Device* device, unsigned int size);

	//! Sets the constant buffer to a vertex shader
	/*!
	\param devcon DirectX Device Context pointer
	*/
	void vsSet(ID3D11DeviceContext* devcon);
	//! Sets the constant buffer to a pixel shader
	/*!
	\param devcon DirectX Device Context pointer
	*/
	void psSet(ID3D11DeviceContext* devcon);
	//! Sets the constant buffer to a compute shader
	/*!
	\param devcon DirectX Device Context pointer
	*/
	void csSet(ID3D11DeviceContext* devcon);
protected:
	ID3D11Buffer* constantBuffer_; //!< DirectX buffer object containing a constant buffer.
};

#endif //XKILL_RENDERER_CB_H