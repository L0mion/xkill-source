#ifndef XKILL_RENDERER_CONSTANTBUFFERS_HLSL
#define XKILL_RENDERER_CONSTANTBUFFERS_HLSL

#pragma pack_matrix(row_major)

#define MAX_NUM_BONES 64

cbuffer cbPerInstance : register (b0)
{
	uint screenWidth;
	uint screenHeight;
	uint pad1;
	uint pad2;
};

cbuffer cbPerFrame : register (b1)
{
	uint numLightsDir;
	uint numLightsPoint;
	uint numLightsSpot;
	uint pad3;
};

cbuffer cbPerCamera : register (b2)
{
	row_major float4x4	view;
	row_major float4x4	viewInverse;
	row_major float4x4	projection;
	row_major float4x4	projectionInverse;
	float3		eyePosition;
	uint		viewportTopX;

	uint		viewportTopY;
	uint		zNear;
	uint		zFar;
	uint		pad4;
};

cbuffer cbPerObject : register (b3)
{
	row_major float4x4 finalMatrix;
	row_major float4x4 worldMatrix;
	row_major float4x4 worldMatrixInverse;
};

cbuffer cbPerSubset : register (b4)
{
	float3	specularTerm;
	float	specularPower;
};

cbuffer cbBones : register (b5)
{
	float4x4 boneTransforms[MAX_NUM_BONES];
};

cbuffer cbSprite : register (b6)
{
	float4x4 spriteMatrix;
}

#endif //XKILL_RENDERER_CONSTANTBUFFERS_HLSL