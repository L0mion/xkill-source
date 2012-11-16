
#include "structs.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 diffuse	: SV_TARGET1;
};

SamplerState ss : register(s0);

PSOut defaultPixelShader(VSOut pIn)
{
	float4 white = float4(1.0f, 1.0f, 1.0f, 1.0f);

	PSOut output;
	output.normal = white;
	output.diffuse = white;

	return output;
}