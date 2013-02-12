
#include "VSOut.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
};

Texture2D gBufferNormal : register( t0 );
Texture2D gBufferAlbedo : register( t1 );

SamplerState ss : register(s0);

float4 PS_DefaultDeferred(DefaultVSOut pIn) : SV_TARGET
{
	float4 color = gBufferAlbedo.Sample(ss, pIn.texcoord);

	return color;
}