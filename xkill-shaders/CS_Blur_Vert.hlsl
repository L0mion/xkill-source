#ifndef XKILL_RENDERER_CS_BLUR_VERT_HLSL
#define XKILL_RENDERER_CS_BLUR_VERT_HLSL

#include "CS_Blur.hlsl"

[numthreads(1, N, 1)]
void CS_Blur_Vert(
	int3	threadIDDispatch	: SV_DispatchThreadID,	//dispathThreadID
	int3	threadIDBlock		: SV_GroupThreadID)		//groupThreadID
{
}

#endif //XKILL_RENDERER_CS_BLUR_VERT_HLSL