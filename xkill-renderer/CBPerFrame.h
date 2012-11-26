#ifndef XKILL_RENDERER_CBPERFRAME_H
#define XKILL_RENDERER_CBPERFRAME_H

#include <DirectXMath.h>

#include "CB.h"

#include "MathBasic.h"

//! Class for a constant buffer that will be updated every frame.
/*!
\ingroup xkill-renderer-constant-buffer
*/
class CBPerFrame : public CB
{
public:
	//! Sets CBPerFrame to its default state.
	CBPerFrame();
	//! Releases all memory and returns CBPerFrame to default state
	virtual ~CBPerFrame();
	//! Releases all memory and returns CBPerFrame to default state
	virtual void reset();
	//! Updates the constant buffer with new data.
	/*!
	\param devcon DirectX Decive Context pointer.
	\param worldViewProj Final transformation matrix that will be used by the shader.
	*/
	void update(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 worldViewProj);
};

#endif //XKILL_RENDERER_CBPERFRAME_H