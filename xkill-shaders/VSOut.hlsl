#ifndef XKILL_RENDERER_VSOUT_HLSL
#define XKILL_RENDERER_VSOUT_HLSL

struct VSOutPosNormVTex
{
	float4 position		: SV_POSITION;
	float3 normalV		: NORMAL;
	float2 texcoord		: TEXCOORD;
};

struct ColorVSOut
{
	float4 position : SV_POSITION;
	float3 color	: COLOR;
};

struct SpriteVSOut
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texcoord	: TEXCOORD;
};

struct VSOutPosNormVTexTanW
{
	float4 position		: SV_POSITION;
	float3 normalV		: NORMAL;
	float2 texcoord		: TEXCOORD;
	float3 tangentW		: TANGENT;
};

struct VSOutPosNormVTexTanWGlowMod
{
	float4 position		: SV_POSITION;
	float3 normalV		: NORMAL;
	float2 texcoord		: TEXCOORD;
	float3 tangentW		: TANGENT;
	float3 glowmod		: GLOWMOD;
};

struct VSOutPosTex
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

#endif //XKILL_RENDERER_VSOUT_HLSL