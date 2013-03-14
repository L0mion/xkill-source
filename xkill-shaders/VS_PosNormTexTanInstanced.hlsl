#ifndef XKILL_RENDERER_VS_POSNORMTEXTANINSTANCED_HLSL
#define XKILL_RENDERER_VS_POSNORMTEXTANINSTANCED_HLSL

#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

VSOutPosNormVTexTanWGlowMod VS_PosNormTexTanInstanced(VSInPosNormTexTanInstanced vsIn)
{
	VSOutPosNormVTexTanWGlowMod output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.position	= pos;

	float4 normal = float4(vsIn.normal, 0.0f);
	normal = mul(normal, vsIn.world);
	normal = mul(normal, view);
	output.normalV = normal.xyz;

	output.tangentW = mul(vsIn.tangent.xyz, (float3x3)vsIn.world);

	output.texcoord	= vsIn.texcoord;

	output.glowmod = vsIn.glowmod;

	return output;
}

#endif //XKILL_RENDERER_VS_POSNORMTEXTANINSTANCED_HLSL