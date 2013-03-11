#ifndef XKILL_RENDERER_CS_SSAO_HLSL
#define XKILL_RENDERER_CS_SSAO_HLSL

#include "constantBuffers.hlsl"
#include "UtilReconstructPosition.hlsl"

#define SSAO_BLOCK_DIM	32
#define RANDOM_DIM		64

//Global Memory
RWTexture2D<float4> ssao : register( u1 );

Texture2D gBufferNormal : register( t0	);
Texture2D bufferDepth	: register(	t6	);
Texture2D bufferRandom	: register( t11	);

SamplerState ssNormal	: register( s0 );
SamplerState ssDepth	: register( s1 );
SamplerState ssRandom	: register( s2 );

//Shared Memory
groupshared float depth[SSAO_BLOCK_DIM][SSAO_BLOCK_DIM];

float occlusionFunc(float2 texGlobal, float2 texOffset, float3 occludee, float3 occludeeNormal)
{
	float2 dispatch = (texGlobal + texOffset); 
	dispatch.x *= ssaoWidth;	dispatch.x -= viewportTopX;
	dispatch.y *= ssaoHeight;	dispatch.y -= viewportTopY;
	float2 texLocal = float2(
			dispatch.x / viewportWidth, 
			dispatch.y / viewportHeight);

	//Get view-space position of occluder:
	float occluderDepth = bufferDepth.SampleLevel(ssDepth, texGlobal + texOffset, 0).x;
	float3 occluder = UtilReconstructPositionViewSpace(
		texLocal, //texGlobal + texOffset
		occluderDepth, 
		projectionInverse);

	//Clip samples with large depth-differences.
	//if(abs(occludee.z - occluder.z) > occlusionRadius) //Consider putting this condition in a seperate var?
	//	return 0.0f;

	//Get vector pointing to occluder.
	float3 diff = occluder - occludee;
	const float3 v = normalize(diff);

	//Get distance to occluder.
	const float d = length(diff) * occlusionScale;

	//Apply occlusion-formulae.
	return max(0.0f, dot(occludeeNormal, v) - occlusionBias) * (1.0f / (1.0f + d)) * occlusionIntensity;
}

[numthreads(SSAO_BLOCK_DIM, SSAO_BLOCK_DIM, 1)]
void CS_SSAO(
	uint3 threadIDDispatch	: SV_DispatchThreadID)
{
	//Convert [0-ssaoWidth, 0-ssaoHeight] -> [0, 1]
	const float2 texCoord = float2(
		(float)(threadIDDispatch.x + viewportTopX)	/ (float)ssaoWidth,
		(float)(threadIDDispatch.y + viewportTopY)	/ (float)ssaoHeight);

	//Get view-space position of occluded point:
	const float occludeeDepth = bufferDepth.SampleLevel(ssDepth, texCoord, 0).x;
	const float3 occludee = UtilReconstructPositionViewSpace(
		float2(
			(float)threadIDDispatch.x / viewportWidth,
			(float)threadIDDispatch.y / viewportHeight), //Convert [0-ssaoWidth, 0-ssaoHeight] -> [0, 1]
		occludeeDepth, 
		projectionInverse);

	//Get view-space normal of occluded point:
	float3 normal = gBufferNormal.SampleLevel(ssNormal, texCoord, 0).xyz;
	normal.x *= 2.0f; normal.x -= 1.0f;
	normal.y *= 2.0f; normal.y -= 1.0f;
	normal.z *= 2.0f; normal.z -= 1.0f;
	//Normal is already in view-space
	const float3 occludeeNormal = normalize(normal); //mul(float4(normal, 0.0f), view).xyz

	//Random texture contains random normalized vectors.
	const float2 scaleTile = float2((float)ssaoWidth / (float)RANDOM_DIM, (float)ssaoHeight / (float)RANDOM_DIM); //Move me into a constant buffer.
	float3 random = bufferRandom.SampleLevel(ssRandom, texCoord * scaleTile, 0).rgb; //Tile random-texture using scaleTile.
	random *= 2.0f; random -= 1.0f; //Map from [0, 1] to [-1, +1]
	random = normalize(random);

	//Sample kernel
	const float2 vec[4] =
	{
		float2( 1,  0),
		float2(-1,  0),
		float2( 0,  1),
		float2( 0, -1)
	};

	//Sampling radius is scaled as geometry is further away.
	float radius = occlusionRadius;
	if(occludee.z > 1.0f)
		 radius /= occludee.z;

	float occlusion = 0.0f;
	const unsigned int numSamples = 4;
	for(unsigned int i = 0.0f; i < numSamples; i++)
	{
		float2 coord1 = reflect(vec[i], random.xy) * radius;	//Original sampling coordinates at 90 degrees.
		float2 coord2 = float2(
		coord1.x * 0.707f - coord1.y * 0.707f,
		coord1.x * 0.707f + coord1.y * 0.707f);			//...the same coordinates rotated 45 degrees:
	
		occlusion += occlusionFunc(texCoord, coord1 * 0.25f, occludee, occludeeNormal);
		occlusion += occlusionFunc(texCoord, coord2 * 0.5f,	 occludee, occludeeNormal);
		occlusion += occlusionFunc(texCoord, coord1 * 0.75f, occludee, occludeeNormal);
		occlusion += occlusionFunc(texCoord, coord2 * 1.0f,	 occludee, occludeeNormal);
	}
	occlusion /= numSamples * 4.0f;

	occlusion = 1.0f - occlusion;
	occlusion = saturate(pow(occlusion, 4.0f)); //Make occlusion appear more dramatic.
	ssao[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		occlusion;
}

#endif //XKILL_RENDERER_CS_SSAO_HLSL