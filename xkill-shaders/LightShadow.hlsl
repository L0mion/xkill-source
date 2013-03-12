#ifndef XKILL_RENDERER_LIGHTSHADOW_HLSL
#define XKILL_RENDERER_LIGHTSHADOW_HLSL

static const unsigned int	SHADOWMAP_SIZE				= 1024; //Dimensions of shadowmap.
static const unsigned int	SHADOWMAP_NUM_PCF_SAMPLES	= 9;
static const float			SHADOWMAP_DX				= 1.0f / (float)SHADOWMAP_SIZE;

float LightShadow(
	SamplerComparisonState ssShadow,
	Texture2D shadowMap,
	float4 shadowPosH)
{
	shadowPosH.xyz /= shadowPosH.w; //Complete projection by doing division by w.

	const float depth = shadowPosH.z; //Depth in NDC-space.

	//Prepare PCF-Filtering:
	const float2 offsets[SHADOWMAP_NUM_PCF_SAMPLES] = 
	{
		float2(-SHADOWMAP_DX, -SHADOWMAP_DX),	float2(0.0f, -SHADOWMAP_DX),	float2(SHADOWMAP_DX, -SHADOWMAP_DX),
		float2(-SHADOWMAP_DX, 0.0f),			float2(0.0f, 0.0f),				float2(SHADOWMAP_DX, 0.0f),
		float2(-SHADOWMAP_DX, +SHADOWMAP_DX),	float2(0.0f, +SHADOWMAP_DX),	float2(SHADOWMAP_DX, +SHADOWMAP_DX)
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