
#include "constantBuffers.hlsl"
#include "VSOut.hlsl"

Texture2D tex : register (t0);

SamplerState ss : register(s0);

float4 PS_Sprite(SpriteVSOut pIn) : SV_TARGET
{
	float4 color = tex.Sample(ss, pIn.texcoord);

	clip(color.a -0.5);

	return color;
}