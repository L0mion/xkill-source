#ifndef XKILL_RENDERER_VS_POSNORMTEXTANINSTANCED_HLSL
#define XKILL_RENDERER_VS_POSNORMTEXTANINSTANCED_HLSL

#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

VSOutPosNormWTexTanW VS_PosNormTexTanInstanced(VSInPosNormTexTanInstanced vsIn)
{
	VSOutPosNormWTexTanW output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.position	= pos;

	output.normalW = mul(float4(vsIn.normal, 0.0f), vsIn.world).xyz;

	output.tangentW = mul(vsIn.tangent.xyz, (float3x3)vsIn.world);

	output.texcoord	= vsIn.texcoord;

	return output;
}

#endif //XKILL_RENDERER_VS_POSNORMTEXTANINSTANCED_HLSL