#ifndef XKILL_RENDERER_STRUCTS_HLSL
#define XKILL_RENDERER_STRUCTS_HLSL

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

struct ColorVSIn
{
	float3 position : POSITION;
	float3 color	: COLOR;
};
struct ColorVSOut
{
	float4 position : SV_POSITION;
	float3 color	: COLOR;
};

struct AnimationVSIn
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texcoord		: TEXCOORD;
	float4 tangent		: TANGENT;
	float3 weights		: WEIGHTS;
	uint4  boneIndices	: BONEINDICES;
};

struct quad_vertex
{
	uint id : SV_VertexID;
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

#endif //XKILL_RENDERER_STRUCTS_HLSL