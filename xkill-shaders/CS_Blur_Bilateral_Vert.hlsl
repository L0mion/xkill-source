#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_VERT_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_VERT_HLSL

#include "CS_Blur.hlsl"

[numthreads(1, N, 1)]
void CS_Blur_Bilateral_Vert(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID)
{
}

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_VERT_HLSL