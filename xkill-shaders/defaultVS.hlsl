
#include "constantBuffers.hlsl"
#include "structs.hlsl"

DefaultVSOut defaultVS(DefaultVSInInstanced vsIn)
{
	DefaultVSOut output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(vsIn.world,	pos);
	pos = mul(view,			pos);
	pos = mul(projection,	pos);
	
	output.position		= pos;
	output.positionW	= mul(vsIn.world, float4(vsIn.position, 1.0f)).xyz;
	output.normalW		= mul(vsIn.world, float4(vsIn.normal, 0.0f)).xyz;
	output.texcoord		= vsIn.texcoord;

	return output;
}