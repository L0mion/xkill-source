#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL

#include "CS_Blur.hlsl"

SamplerState ssNormal	: register( s0 );
SamplerState ssDepth	: register( s1 );

//Global
Texture2D gBufferNormal : register( t0	);
Texture2D bufferDepth	: register(	t6	);

//Shared
groupshared float	sharedDepth[sharedCacheSize];	
groupshared float3	sharedNormV[sharedCacheSize];

float3 GetNormalV(float3 normalW)
{
	normalW.x *= 2.0f; normalW.x -= 1.0f;
	normalW.y *= 2.0f; normalW.y -= 1.0f;
	normalW.z *= 2.0f; normalW.z -= 1.0f;

	return normalize(mul(float4(normalW, 0.0f), view).xyz);
}
float2 GetTexCoordGlobal(int2 xy)
{
	return float2(
		(float)(xy.x + viewportTopX) / (float)ssaoWidth,
		(float)(xy.y + viewportTopY) / (float)ssaoHeight);
}

void CacheData(int index, int2 xy)
{
	float2 xyGlobal = GetTexCoordGlobal(xy);

	sharedCache[index] = toBlur[xy];
	sharedDepth[index] = bufferDepth				.SampleLevel(ssDepth,	xyGlobal, 0).x;
	sharedNormV[index] = GetNormalV(gBufferNormal	.SampleLevel(ssNormal,	xyGlobal, 0).xyz);
}

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
		xy);
	
	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.x < blurRadius)
	{
		xy = int2(
			max(threadIDDispatch.x - blurRadius, 0), //Clamp out of bound samples that occur at image borders. 
			threadIDDispatch.y);
		CacheData(
			threadIDBlock.x,
			xy);
	}
	if(threadIDBlock.x >= N - blurRadius)
	{
		xy = int2(
			min(threadIDDispatch.x + blurRadius, toBlur.Length.x - 1), //Clamp out of bound samples that occur at image borders.
			threadIDDispatch.y);
		CacheData(
			threadIDBlock.x + 2 * blurRadius,
			xy);
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
	blurred[threadIDDispatch.xy] = blur;
}

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL