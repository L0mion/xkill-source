
#include "structs.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
};

Texture2D texAlbedo : register(t0);
Texture2D texNormal : register(t1);

SamplerState ss : register(s0);

PSOut defaultPS(VSOut pIn)
{
	float4 white = float4(0.0f, 1.0f, 1.0f, 1.0f);

	PSOut output;
	output.normal.xyz	= pIn.normalW;
	output.albedo		= texNormal.SampleLevel(ss, pIn.texcoord, 0); //white;

	return output;
}