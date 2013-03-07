#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_HLSL

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

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_HLSL