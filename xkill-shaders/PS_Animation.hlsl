#include "VSOut.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
};

PSOut PS_Animation(VSOutPosNormVTex pIn)
{
	float4 white = float4(0.75f, 0.0f, 0.0f, 1.0f);

	PSOut output;
	output.normal = float4(pIn.normalV, 1.0f);
	output.albedo = white;

	return output;
}