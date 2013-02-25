#ifndef XKILL_RENDERER_PS_DOWNSAMPLE_HLSL
#define XKILL_RENDERER_PS_DOWNSAMPLE_HLSL

#include "VSOut.hlsl"

struct PSOut
{
	float4 lowTex : SV_TARGET0;
};

Texture2D highTex : register( t3 ); //Register shared in CS_Lighting

SamplerState ss : register(s0);

PSOut PS_DownSample(VSOutPosTex pIn)
{
	PSOut psOut;
	psOut.lowTex = highTex.Sample(ss, pIn.texcoord);

	return psOut;
}

#endif //XKILL_RENDERER_PS_DOWNSAMPLE_HLSL