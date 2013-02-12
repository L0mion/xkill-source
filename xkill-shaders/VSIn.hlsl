#ifndef XKILL_RENDERER_VSIN_HLSL
#define XKILL_RENDERER_VSIN_HLSL

struct VSInPosNormTexInstanced
{
	float3		position	: POSITION;
	float3		normal		: NORMAL;
	float2		texcoord	: TEXCOORD;
	float4x4	world		: WORLD;
};

struct VSInPosColor
{
	float3 position : POSITION;
	float3 color	: COLOR;
};

struct VSInPosNormTex
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD;
};

struct VSInPosNormTexTanSkinned
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 texcoord		: TEXCOORD;
	float4 tangent		: TANGENT;
	float3 weights		: WEIGHTS;
	uint4  boneIndices	: BONEINDICES;
};

struct VSInPosNormTexTanInstanced
{
	float3		position	: POSITION;
	float3		normal		: NORMAL;
	float2		texcoord	: TEXCOORD;
	float4		tangent		: TANGENT;
	float4x4	world		: WORLD;
};

#endif //XKILL_RENDERER_VSIN_HLSL