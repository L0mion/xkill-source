#ifndef XKILL_RENDERER_CONSTANTBUFFERS_H
#define XKILL_RENDERER_CONSTANTBUFFERS_H

#include <DirectXMath.h>

#include "mathBasic.h"

/*! \defgroup xkill-renderer-constant-buffers xkill-renderer-constant-buffers
	\ingroup xkill-renderer
	Constant buffers transferring data to shaders in batches. */

//! Per-Frame constant buffer
/*!
Contains data that will be sent to shader every frame.
\ingroup xkill-renderer-constant-buffers
*/
struct CBPerFrameDesc
{
	DirectX::XMFLOAT4X4 worldViewProj_; //!< Final matrix.
};
//! Size of Per-Frame constant buffer. Must have a size of a multiple of 16 bytes.
/*!
\ingroup xkill-renderer-constant-buffers
*/
static const unsigned int cbPerFrameSize = 64;

//! Per-Instance constant buffer
/*!
Contains data that will be sent to shader for every new instance.
\ingroup xkill-renderer-constant-buffers
*/
struct CBPerInstanceDesc
{
	unsigned int screenWidth_;
	unsigned int screenHeight_;
	unsigned int tileWidth_;
	unsigned int tileHeight_;
};
//! Size of Per-Instance constant buffer. Transfers data only changed during initialization to shader. Must have a size of a multiple of 16 bytes.
/*!
\ingroup xkill-renderer-constant-buffers
*/
static const unsigned int cbPerInstanceSize = 64;

#endif //XKILL_RENDERER_CONSTANTBUFFER_H