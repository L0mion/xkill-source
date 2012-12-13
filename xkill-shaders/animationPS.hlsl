#include "structs.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
};

SamplerState ss : register(s0);

PSOut animationPS(VSOut pIn)
{
	float4 white = float4(0.75f, 0.0f, 0.0f, 1.0f);

	PSOut output;
	output.normal.xyz = pIn.normalW;
	output.albedo = white;

	return output;
}