#ifndef XKILL_RENDERER_CONSTANTBUFFERS_H
#define XKILL_RENDERER_CONSTANTBUFFERS_H

#include "mathBasic.h"

//! Per-Frame constant buffer
/*!
Contains data that will be sent to shader every frame.
\ingroup xkill-renderer
*/
struct CBPerFrame
{
	MatF4 worldViewProj; //!< Final matrix.
};
static const unsigned int cbPerFrameSize = 64; //!< Size of Per-Frame constant buffer. Must have a size of a multiple of 16 bytes.

#endif //XKILL_RENDERER_CONSTANTBUFFER_H