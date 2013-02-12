#ifndef XKILL_RENDERER_VS_POSNORMTEXINSTANCED_HLSL
#define XKILL_RENDERER_VS_POSNORMTEXINSTANCED_HLSL

#include "constantBuffers.hlsl"
#include "VSOut.hlsl"
#include "VSIn.hlsl"

DefaultVSOut VS_PosNormTexInstanced(VSInPosNormTexInstanced vsIn)
{
	DefaultVSOut output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	output.positionW = pos.xyz;
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.position	= pos;

	float3 normal = vsIn.normal;
	normal = mul(float4(normal, 0.0f), vsIn.world).xyz;
	//normal = mul(float4(normal, 0.0f), view).xyz;
	output.normalW = normal;

	output.texcoord	= vsIn.texcoord;

	return output;
}

#endif //XKILL_RENDERER_VS_POSNORMTEXINSTANCED_HLSL