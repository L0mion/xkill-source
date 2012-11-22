
struct VSOut
{
	float2 texcoord		: TEXCOORD;
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float3 normalW		: NORMAL;
};

struct DefaultVSIn
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 texcoord	: TEXCOORD;
};

struct quad_vertex
{
	uint id : SV_VertexID;
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};