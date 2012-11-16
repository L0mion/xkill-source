
struct VSOut
{
	float2 texcoord : TEXCOORD;
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
};