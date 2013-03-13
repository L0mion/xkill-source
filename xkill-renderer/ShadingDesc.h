#ifndef XKILL_RENDERER_SHADINGDESC_H
#define XKILL_RENDERER_SHADINGDESC_H

typedef unsigned int UINT;

#include "shader.h"
#include "ManagementFX.h"

struct ShadingDesc
{
	ShadingDesc();
	~ShadingDesc();

	ShaderID vsID_;
	ShaderID gsID_;
	ShaderID psID_;

	LayoutID layoutID_;

	UINT stride_[2];
};

#endif //XKILL_RENDERER_SHADINGDESC_H