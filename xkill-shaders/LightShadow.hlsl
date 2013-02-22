#ifndef XKILL_RENDERER_LIGHTSHADOW_HLSL
#define XKILL_RENDERER_LIGHTSHADOW_HLSL

static const unsigned int SHADOWMAP_SIZE = 1024; //Dimensions of shadowmap.
static const float SHADOWMAP_DX	= 1.0f / (float)SHADOWMAP_SIZE;
static const unsigned int SHADOWMAP_NUM_PCF_SAMPLES = 9;

float LightShadow(
	SamplerComparisonState ssShadow,
	Texture2D shadowMap,
	float4 shadowPosH)
{
	shadowPosH.xyz /= shadowPosH.w; //Complete projection by doing division by w.

	float depth = shadowPosH.z; //Depth in NDC-space.

	const float dx = SHADOWMAP_DX; //Texel size.

	//Prepare PCF-Filtering:
	const float2 offsets[SHADOWMAP_NUM_PCF_SAMPLES] = 
	{
		float2(-dx, -dx),	float2(0.0f, -dx),	float2(dx, -dx),
		float2(-dx, 0.0f),	float2(0.0f, 0.0f),	float2(dx, 0.0f),
		float2(-dx, +dx),	float2(0.0f, +dx),	float2(dx, +dx)
	};

	//Box Filter 3x3 PCF-Filtering:
	float percentLit = 0.0f;
	[unroll] for(unsigned int i = 0; i < SHADOWMAP_NUM_PCF_SAMPLES; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(
			ssShadow, 
			shadowPosH.xy + offsets[i], 
			depth).r; //Hardware support for PCF
	}

	//Average the samples.
	return percentLit /= (float)SHADOWMAP_NUM_PCF_SAMPLES;
}

#endif //XKILL_RENDERER_LIGHTSHADOW_HLSL