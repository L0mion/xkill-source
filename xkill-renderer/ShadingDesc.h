#ifndef XKILL_RENDERER_SHADINGDESC_H
#define XKILL_RENDERER_SHADINGDESC_H

#include "shader.h"
#include "ManagementFX.h"

struct ShadingDesc
{
	ShadingDesc();
	~ShadingDesc();

	ShaderID vsID_;
	ShaderID psID_;

	LayoutID layoutID_;

	UINT stride_[2];
};

#endif //XKILL_RENDERER_SHADINGDESC_H