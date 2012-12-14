#ifndef XKILL_RENDERER_CONSTANTBUFFERS_HLSL
#define XKILL_RENDERER_CONSTANTBUFFERS_HLSL

#define MAX_NUM_BONES 96

cbuffer cbPerInstance : register (b0)
{
	uint screenWidth;
	uint screenHeight;
	uint pad1;
	uint pad2;
};

cbuffer cbPerFrame : register (b1)
{
	uint numLights;
	uint pad3;
	uint pad4;
	uint pad5;
};

cbuffer cbPerCamera : register (b2)
{
	float4x4	view;
	float4x4	viewInverse;
	float4x4	projection;
	float4x4	projectionInverse;
	float3		eyePosition;
	uint		viewportTopX;
	uint		viewportTopY;
};

cbuffer cbPerObject : register (b3)
{
	float4x4 finalMatrix;
	float4x4 worldMatrix;
	float4x4 worldMatrixInverse;
};

cbuffer cbBones : register (b4)
{
	float4x4 boneTransforms[MAX_NUM_BONES];
}

#endif //XKILL_RENDERER_CONSTANTBUFFERS_HLSL