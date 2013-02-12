#ifndef XKILL_RENDERER_VSOUT_HLSL
#define XKILL_RENDERER_VSOUT_HLSL

struct DefaultVSOut
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float3 normalW		: NORMAL;
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

#endif //XKILL_RENDERER_VSOUT_HLSL