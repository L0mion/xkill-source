#ifndef XKILL_RENDERER_CS_SSAO_HLSL
#define XKILL_RENDERER_CS_SSAO_HLSL

#include "constantBuffers.hlsl"
#include "UtilReconstructPosition.hlsl"

//Global Memory
RWTexture2D<float4> ssao : register( u1 );

Texture2D gBufferNormal : register( t0	);
Texture2D bufferDepth	: register(	t6	);
Texture2D bufferRandom	: register( t11	);

SamplerState ssNormal	: register( s0 );
SamplerState ssDepth	: register( s1 );
SamplerState ssRandom	: register( s2 );

#define SSAO_BLOCK_DIM	16
#define RANDOM_DIM		4

[numthreads(SSAO_BLOCK_DIM, SSAO_BLOCK_DIM, 1)]
void CS_SSAO(
	//uint3	blockID				: SV_GroupID,
	//uint	threadIDBlockIndex	: SV_GroupIndex,
	uint3	threadIDDispatch	: SV_DispatchThreadID
	//uint3	threadIDBlock		: SV_GroupThreadID,
	)
{
	float2 texCoord = float2(
		(float)(threadIDDispatch.x + viewportTopX)	/ (float)ssaoWidth,
		(float)(threadIDDispatch.y + viewportTopY)	/ (float)ssaoHeight);

	//Get view-space position of occluded point:
	float occludeeDepth = bufferDepth.SampleLevel(ssDepth, texCoord, 0).x;
	float3 occludee = UtilReconstructPositionViewSpace(
		float2((float)threadIDDispatch.x / viewportWidth, (float)threadIDDispatch.y / viewportHeight),
		occludeeDepth, 
		projectionInverse); //Should we use current viewports here - or the viewport dimensions previously used?

	//Get view-space normal of occluded point:
	float3 normal = gBufferNormal.SampleLevel(ssNormal, texCoord, 0).xyz;
	normal.x *= 2.0f; normal.x -= 1.0f;
	normal.y *= 2.0f; normal.y -= 1.0f;
	normal.z *= 2.0f; normal.z -= 1.0f;
	float3 occludeeNormal = normalize(mul(float4(normal, 0.0f), view).xyz);

	float2 scaleTile = float2((float)ssaoWidth / (float)RANDOM_DIM, (float)ssaoHeight / (float)RANDOM_DIM); //Move me into a constant buffer.
	float3 random = bufferRandom.SampleLevel(ssRandom, texCoord * scaleTile, 0).rgb; //Tile random-texture using scaleTile.
	random *= 2.0f; random -= 1.0f; //Map from [0, 1] to [-1, +1]
	random = normalize(random);

	//--------------
	//Purpose of this next bit of code is to establish an orthonormal change-of-basis matrix which will
	//re-orient offset-kernel along occludeeNormal. Incorporated here is also the random rotation of sample hemisphere.
	//--------------
	// * occludeeNormal and random make up the normalized two-dimensional basis V2.
	// * Use Grahm-Schmidt to establish three-dimensional orthonormal basis V3.
	float3 tangent		= normalize(random - occludeeNormal * dot(random, occludeeNormal));
	float3 bitangent	= cross(occludeeNormal, tangent);
	float3x3 tbn		= float3x3(tangent, bitangent, occludeeNormal);

	float occlusion	= 0.0f;
	float radius	= 25.0f;//occlusionRadius; //removeme
	for(unsigned int i = 0; i < 14; i++)
	{
		//Get view-space position of occluding point:
		float3 occluder = mul(offsetKernel[i].xyz, tbn);
		occluder = occluder * radius + occludee;

		//Project the view-space occluder into screen-space in order to get texture-coordinates:
		float4 offset = float4(occluder, 1.0f);
		offset = mul(offset, projection);
		offset.xy /= offset.w; //Complete projection.
		offset.xy = offset.xy * 0.5f + 0.5f; //Convert from [-1, +1] to [0, 1].

		//Sample depth and re-construct view-space position of point in order to get view-space depth:
		float occluderDepth = bufferDepth.SampleLevel(ssDepth, offset.xy, 0).x;
		float3 occluderV = UtilReconstructPositionViewSpace(
			float2(offset.xy),
			occluderDepth, 
			projectionInverse);

		//Range-check is used to avoid gaining occlusion from big depth-differences at edges of geometry.
		float rangeCheck = 0.0f;
		if(abs(occludee.z - occluderV.z) < radius)
		{
			rangeCheck = 1.0f;
		}
		float occlusionFactor = 0.0f;
		if(occluderV.z <= occluder.z)
		{
			occlusionFactor = 1.0f;
		}
		occlusion += occlusionFactor * rangeCheck;
	}

	occlusion = 1.0f - (occlusion / 14); //saturate(pow(access, 4.0f))
	ssao[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(occlusion, 1.0f, 1.0f, 1.0f);
}

#endif //XKILL_RENDERER_CS_SSAO_HLSL