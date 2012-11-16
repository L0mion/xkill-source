
#include "structs.hlsl"

VSOut defaultVS(float3 position : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD)
{
	VSOut output;
	output.position = float4(position, 1.0f);
	output.positionW = position;
	output.normalW = normal;
	output.texcoord = texcoord;

	return output;
}