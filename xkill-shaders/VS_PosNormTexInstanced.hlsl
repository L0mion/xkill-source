#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

VSOutPosNormWTex VS_PosNormTexInstanced(VSInPosNormTexInstanced vsIn)
{
	VSOutPosNormWTex output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.position	= pos;

	output.normalW = mul(float4(vsIn.normal, 0.0f), vsIn.world).xyz;

	output.texcoord	= vsIn.texcoord;

	return output;
}