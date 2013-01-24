#include "LightDesc.hlsl"
#include "constantBuffers.hlsl"
#include "utilities.hlsl"

#define TILE_DIM		16
#define TILE_MAX_LIGHTS	10

//Global memory
RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal						: register( t0 );
Texture2D gBufferAlbedo						: register( t1 );
Texture2D gBufferMaterial					: register( t2 );
Texture2D gBufferDepth						: register( t3 );
StructuredBuffer<LightDir>		lightsDir	: register( t4 );
StructuredBuffer<LightPoint>	lightsPoint	: register( t5 );
StructuredBuffer<LightSpot>		lightsSpot	: register( t6 );
StructuredBuffer<float3>		lightsPos	: register( t7 );

SamplerState ss : register(s0);

[numthreads(TILE_DIM, TILE_DIM, 1)]
void lightingCS(
	uint3	blockID				: SV_GroupID,
	uint	threadIDBlockIndex	: SV_GroupIndex,
	uint3	threadIDDispatch	: SV_DispatchThreadID,
	uint3	threadIDBlock		: SV_GroupThreadID)
{
	//Sample G-Buffers. Data prefetching?
	float2	texCoord	= float2((float)(threadIDDispatch.x + viewportTopX)/(float)screenWidth,(float)(threadIDDispatch.y + viewportTopY)/(float)screenHeight);
	float4	gAlbedo		= gBufferAlbedo		.SampleLevel(ss, texCoord, 0);
	float4	gNormal		= gBufferNormal		.SampleLevel(ss, texCoord, 0);
	float4	gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0);
	float	gDepth		= gBufferDepth		.SampleLevel(ss, texCoord, 0).x;

	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = gAlbedo;
}