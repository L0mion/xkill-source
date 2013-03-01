#include "VSOut.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
};

PSOut PS_Animation(VSOutPosNormWTex pIn)
{
	float4 color = float4(0.75f, 0.0f, 0.0f, 1.0f);

	PSOut output;
	output.normal = float4(pIn.normalW, 1.0f);
	output.albedo = color;

	return output;
}