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

struct quad_vertex
{
	uint id : SV_VertexID;
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

struct Light
{
	float3 position;
	float range;
	float3 direction;
	float spotPower;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 attenuation;
	uint type;
};

struct SurfaceInfo
{
	float3 position;
	float3 normal;
	float4 diffuse;
	float4 specular;
};

#endif //XKILL_RENDERERE_STRUCTS_HLSL