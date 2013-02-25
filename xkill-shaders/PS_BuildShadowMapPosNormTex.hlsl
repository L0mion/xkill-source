#ifndef XKILL_RENDERER_PS_BUILDSHADOWMAPPOSNORMTEX_HLSL
#define XKILL_RENDERER_PS_BUILDSHADOWMAPPOSNORMTEX_HLSL

#include "VSOut.hlsl"

//Pixel shader does not return anything as we only mean to fill up our z-buffer with depth-data.
//If we later implement transperancy, a check for this would be implemented here - thusly clipping such pixels.
void PS_BuildShadowMapPosNormTex(VSOutPosNormWTex pIn)
{
	//Do nothing.
}

#endif //XKILL_RENDERER_PS_BUILDSHADOWMAPPOSNORMTEX_HLSL