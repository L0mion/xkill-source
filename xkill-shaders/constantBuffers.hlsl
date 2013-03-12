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
	uint pad0;
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
	float		zNear;
	float		zFar;
	float		viewportWidth;

	float		viewportHeight;
	uint		pad4;
	uint		pad5;
	uint		pad6;
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

#define NUM_BLUR_KERNEL_ELEMENTS_COMPRESSED		3
#define NUM_BLUR_KERNEL_ELEMENTS_UNCOMPRESSED	12
cbuffer cbBlur : register ( b7 )
{
	float4 blurKernelCompressed[NUM_BLUR_KERNEL_ELEMENTS_COMPRESSED];

	// = 
	//{
	//	0.05f, 
	//	0.05f, 
	//	0.1f, 
	//	0.1f, 
	//	0.1f, 
	//	0.2f,
	//	0.1f,
	//	0.1f,
	//	0.1f,
	//	0.05f,
	//	0.05f
	//}; //Must sum to 1.0f if no color should be added or removed from scene.
};

cbuffer cbSSAO : register ( b8 )
{
	uint ssaoWidth;
	uint ssaoHeight;
	float occlusionRadius;
	float occlusionScale;
	
	float occlusionBias;
	float occlusionIntensity;
	uint pad7;
	uint pad8;

	/*
	occlusionScale:		scales distance between occluders and occludee.
    occlusionBias:		controls the width of the occlusion cone considered by the occludee.
    occlusionRadius:	the sampling radius.
    occlusionIntensity: the ao intensity.
	*/
};

cbuffer cbShadow : register ( b9 )
{
	row_major float4x4 shadowMapTransform;
};

#endif //XKILL_RENDERER_CONSTANTBUFFERS_HLSL