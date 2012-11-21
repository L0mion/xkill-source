
#include "structs.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 diffuse	: SV_TARGET1;
};

Texture2D gBufferNormal : register( t0 );
Texture2D gBufferDiffuse : register( t1 );

SamplerState ss : register(s0);

float4 defaultDeferredPS(VSOut pIn) : SV_TARGET
{
	float4 color = gBufferDiffuse.Sample(ss, pIn.texcoord);

	return color;
}