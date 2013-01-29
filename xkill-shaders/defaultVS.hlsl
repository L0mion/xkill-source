#include "constantBuffers.hlsl"
#include "structs.hlsl"

DefaultVSOut defaultVS(DefaultVSInInstanced vsIn)
{
	DefaultVSOut output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	output.positionW = pos;
	pos = mul(pos, view);
	output.positionW = pos;
	pos = mul(pos, projection);
	output.position	= pos;

	output.normalW	= mul(float4(vsIn.normal, 0.0f), vsIn.world).xyz;
	output.texcoord	= vsIn.texcoord;

	return output;
}