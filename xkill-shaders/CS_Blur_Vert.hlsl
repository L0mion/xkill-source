#ifndef XKILL_RENDERER_CS_BLUR_VERT_HLSL
#define XKILL_RENDERER_CS_BLUR_VERT_HLSL

#include "CS_Blur.hlsl"

[numthreads(1, N, 1)]
void CS_Blur_Vert(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID,
	uint	threadIDBlockIndex	: SV_GroupIndex)
{
	if(threadIDBlockIndex == 0)
	{
		blurKernel = ExtractBlurKernel();
	}

	//Load texel into shared memory to reduce memory bandwidth:
	int2 xy = min(
		threadIDDispatch.xy,
		toBlur.Length.xy - 1); //Clamp out of bound samples that occur at image borders.
	sharedCache[threadIDBlock.y + blurRadius] = toBlur[xy];

	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.y < blurRadius)
	{
		int y = max(
			threadIDDispatch.y - blurRadius,
			0); //Clamp out of bound samples that occur at image borders.
		sharedCache[threadIDBlock.y] = toBlur[int2(threadIDDispatch.x, y)];
	}
	if(threadIDBlock.y >= N - blurRadius)
	{
		int y = min(
			threadIDDispatch.y + blurRadius,
			toBlur.Length.y - 1); //Clamp out of bound samples that occur at image borders.
		sharedCache[threadIDBlock.y + 2 * blurRadius] = toBlur[int2(threadIDDispatch.x, y)];
	}
	GroupMemoryBarrierWithGroupSync();

	//Blur pixel:
	float4 blur = float4(0.0f, 0.0f, 0.0f, 1.0f);
	[unroll] for(int i = -blurRadius; i <= blurRadius; i++) //OBS - As HLSL may be strange sometimes, the iterating variable declared in this scope may behave in weird ways. (if a variable of the same name has been declared similarily before. (e.g. unsigned int i)
	{
		int k = threadIDBlock.y + blurRadius + i;
		blur += blurKernel._[i + blurRadius] * sharedCache[k];
	}


	blurred[threadIDDispatch.xy] = blur;
}

#endif //XKILL_RENDERER_CS_BLUR_VERT_HLSL