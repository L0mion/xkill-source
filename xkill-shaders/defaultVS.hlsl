
#include "structs.hlsl"

cbuffer cbPerFrame : register (b0)
{
	float4x4 worldViewProj;
}

VSOut defaultVS(DefaultVSIn vsIn)
{
	VSOut output;

	output.position		= mul(worldViewProj, float4(vsIn.position, 1.0f));
	output.positionW	= vsIn.position;
	output.normalW		= vsIn.normal;
	output.texcoord		= vsIn.texcoord;

	return output;
}