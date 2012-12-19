
#include "constantBuffers.hlsl"
#include "structs.hlsl"

VSOut animationVS(AnimationVSIn vsIn)
{
	VSOut output;
	output.position		= mul(finalMatrix, float4(vsIn.position, 1.0f));
	output.positionW	= mul(worldMatrix, float4(vsIn.position, 1.0f)).xyz;
	output.normalW		= mul(worldMatrix, float4(vsIn.normal, 0.0f)).xyz;
	output.texcoord		= vsIn.texcoord;
	return output;
}