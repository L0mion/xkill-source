#ifndef XKILL_RENDERER_CS_BLUR_HORZ_HLSL
#define XKILL_RENDERER_CS_BLUR_HORZ_HLSL

#include "CS_Blur.hlsl"

[numthreads(N, 1, 1)]
void CS_Blur_Horz(
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
	sharedCache[threadIDBlock.x + blurRadius] = toBlur[xy];
	
	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.x < blurRadius)
	{
		int x = max(
			threadIDDispatch.x - blurRadius, 
			0); //Clamp out of bound samples that occur at image borders.
		sharedCache[threadIDBlock.x] = toBlur[int2(x, threadIDDispatch.y)];
	}
	if(threadIDBlock.x >= N - blurRadius)
	{
		int x = min(
			threadIDDispatch.x + blurRadius, 
			toBlur.Length.x - 1); //Clamp out of bound samples that occur at image borders.
		sharedCache[threadIDBlock.x + 2 * blurRadius] = toBlur[int2(x, threadIDDispatch.y)];
	}
	GroupMemoryBarrierWithGroupSync();
	
	//Blur pixel:
	float4 blur = float4(0.0f, 0.0f, 0.0f, 1.0f);
	[unroll] for(int i = -blurRadius; i <= blurRadius; i++) //OBS - As HLSL may be strange sometimes, the iterating variable declared in this scope may behave in weird ways. (if a variable of the same name has been declared similarily before. (e.g. unsigned int i)
	{
		int k = threadIDBlock.x + blurRadius + i;
		blur += blurKernel._[i + blurRadius] * sharedCache[k];
	}

	blurred[threadIDDispatch.xy] = blur;
}

#endif //XKILL_RENDERER_CS_BLUR_HORZ_HLSL