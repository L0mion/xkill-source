#ifndef XKILL_RENDERER_CONSTANTBUFFERS_H
#define XKILL_RENDERER_CONSTANTBUFFERS_H

#include "mathBasic.h"

struct CBPerFrame
{
	MatF4 worldViewProj;
};
static const unsigned int cbPerFrameSize = 64;

#endif //XKILL_RENDERER_CONSTANTBUFFER_H