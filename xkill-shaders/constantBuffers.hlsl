#ifndef XKILL_RENDERER_CONSTANTBUFFERS_HLSL
#define XKILL_RENDERER_CONSTANTBUFFERS_HLSL

cbuffer cbPerFrame : register (b0)
{
	float4x4	worldViewProj;
	float4x4	view;
	float4x4	viewInverse;
	float4x4	projection;
	float4x4	projectionInverse;
	float3		eyePosition;
	uint		numLights;
}

cbuffer cbPerInstance : register (b1)
{
	uint screenWidth;
	uint screenHeight;
	uint tileWidth;
	uint tileHeight;
}

#endif //XKILL_RENDERER_CONSTANTBUFFERS_HLSL