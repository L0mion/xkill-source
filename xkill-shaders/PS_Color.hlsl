#include "constantBuffers.hlsl"
#include "VSOut.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
};

Texture2D texAlbedo		: register(t0);
Texture2D texNormal		: register(t1);

SamplerState ss : register(s0);

PSOut PS_Color(ColorVSOut colorPSIn)
{
	PSOut output;
	output.albedo = float4(colorPSIn.color, 1.0f);
	output.normal = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return output;
}