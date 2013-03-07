#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL

#include "CS_Blur_Bilateral.hlsl"

[numthreads(N, 1, 1)]
void CS_Blur_Bilateral_Horz(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID)
{
	const BlurKernel blurKernel = ExtractBlurKernel();
	
	//Load texel, depth and view-space normal into shared memory to reduce memory bandwidth:
	int2 xy = min(
		threadIDDispatch.xy, 
		toBlur.Length.xy - 1); //Clamp out of bound samples that occur at image borders.
	CacheData(
		threadIDBlock.x + blurRadius,
		xy + uint2(viewportTopX, viewportTopY));
	
	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.x < blurRadius)
	{
		xy = int2(
			max(threadIDDispatch.x - blurRadius, 0), //Clamp out of bound samples that occur at image borders. 
			threadIDDispatch.y);
		CacheData(
			threadIDBlock.x,
			xy + uint2(viewportTopX, viewportTopY));
	}
	if(threadIDBlock.x >= N - blurRadius)
	{
		xy = int2(
			min(threadIDDispatch.x + blurRadius, toBlur.Length.x - 1), //Clamp out of bound samples that occur at image borders.
			threadIDDispatch.y);
		CacheData(
			threadIDBlock.x + 2 * blurRadius,
			xy + uint2(viewportTopX, viewportTopY));
	}
	GroupMemoryBarrierWithGroupSync();

	int		center			= threadIDBlock.x + blurRadius;
	float3	centerNormal	= sharedNormV[center];
	float	centerDepth		= sharedDepth[center];

	//Center value always contributes to the sum.
	float	totalWeight	= blurKernel._[5];
	float4	blur		= blurKernel._[5] * sharedCache[center];
	for(int i = -blurRadius; i <= blurRadius; i++)
	{
		if(i == 0)
			continue; //ugly

		int		n		= threadIDBlock.x + blurRadius + i;
		float	nDepth	= sharedDepth[n];
		float3	nNormal	= sharedNormV[n];
		float4	nSample	= sharedCache[n];

		if(
			dot(nNormal, centerNormal)	>= 0.8f && 
			abs(nDepth - centerDepth)	<= 0.2f)
		{
			float weight = blurKernel._[i + blurRadius];
			blur		+= weight * nSample;
			totalWeight	+= weight;
		}
	}

	//Compensate discarded samples by making total weights sum to 1.
	blur /= totalWeight;
	blurred[uint2(
		threadIDDispatch.x + viewportTopX,
		threadIDDispatch.y + viewportTopY)] = blur;
}

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL