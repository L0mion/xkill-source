#ifndef XKILL_RENDERER_LIGHTPOS_H
#define XKILL_RENDERER_LIGHTPOS_H

#include <xkill-utilities/Math.h>

struct LightPos
{
	LightPos() { pos_ = Float3(0.0f, 0.0f, 0.0f); }
	LightPos(Float3 pos) { pos_ = pos; }
	~LightPos() { /*Do nothing.*/ }

	Float3 pos_;

	float pad1;
};

#endif //XKILL_RENDERER_LIGHTPOS_H