#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_VERT_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_VERT_HLSL

#include "CS_Blur_Bilateral.hlsl"

[numthreads(1, N, 1)]
void CS_Blur_Bilateral_Vert(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID)
{
	const BlurKernel blurKernel = ExtractBlurKernel();

	//Load texel, depth and view-space normal into shared memory to reduce memory bandwidth:
	int2 xy = min(
		threadIDDispatch.xy, 
		toBlur.Length.xy - 1); //Clamp out of bound samples that occur at image borders.
	CacheData(
		threadIDBlock.y + blurRadius,
		xy + uint2(viewportTopX, viewportTopY));

	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.y < blurRadius)
	{
		xy = int2(
			threadIDDispatch.x, 
			max(threadIDDispatch.y - blurRadius, 0)); //Clamp out of bound samples that occur at image borders. 
		CacheData(
			threadIDBlock.y,
			xy + uint2(viewportTopX, viewportTopY));
	}
	if(threadIDBlock.y >= N - blurRadius)
	{
		xy = int2(
			threadIDDispatch.x, //Clamp out of bound samples that occur at image borders.
			min(threadIDDispatch.y + blurRadius, toBlur.Length.y - 1));
		CacheData(
			threadIDBlock.y + 2 * blurRadius,
			xy + uint2(viewportTopX, viewportTopY));
	}
	GroupMemoryBarrierWithGroupSync();

	int		center			= threadIDBlock.y + blurRadius;
	float3	centerNormal	= sharedNormV[center];
	float	centerDepth		= sharedDepth[center];

	//Center value always contributes to the sum.
	float	totalWeight	= blurKernel._[5];
	float4	blur		= blurKernel._[5] * sharedCache[center];
	for(int i = -blurRadius; i <= blurRadius; i++)
	{
		if(i == 0)
			continue; //ugly

		int		n		= threadIDBlock.y + blurRadius + i;
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

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_VERT_HLSL