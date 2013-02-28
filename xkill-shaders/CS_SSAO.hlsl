#ifndef XKILL_RENDERER_CS_SSAO_HLSL
#define XKILL_RENDERER_CS_SSAO_HLSL

#include "constantBuffers.hlsl"
#include "UtilReconstructPosition.hlsl"

float OcclusionFunction(float distZ)
{
	float occlusion = 0.0f;
	if(distZ > surfaceEpsilon)
	{
		float fadeLength = occlusionFadeEnd - occlusionFadeStart;
		occlusion = saturate((occlusionFadeEnd - distZ) / fadeLength);
	}

	return occlusion;
};

#define SAMPLE_COUNT 14

//Global Memory
RWTexture2D<float4> ssao : register( u1 );

Texture2D gBufferNormal : register( t0	);
Texture2D bufferDepth	: register(	t6	);
Texture2D bufferRandom	: register( t11	);

SamplerState ssNormal	: register( s0 );
SamplerState ssDepth	: register( s1 );
SamplerState ssRandom	: register( s2 );

#define SSAO_BLOCK_DIM 16
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
		float2((float)threadIDDispatch.x / zNear, (float)threadIDDispatch.y / zFar), //Obs, zNear and zFar not what they seem
		occludeeDepth, 
		projectionInverse); 

	//Get view-space normal of occluded point:
	float3 normal = gBufferNormal.SampleLevel(ssNormal, texCoord, 0).xyz;
	normal.x *= 2.0f; normal.x -= 1.0f;
	normal.y *= 2.0f; normal.y -= 1.0f;
	normal.z *= 2.0f; normal.z -= 1.0f;
	float3 occludeeNormal = mul(float4(normal, 0.0f), view).xyz;

	float3 random = bufferRandom.SampleLevel(ssRandom, texCoord, 0).xyz; //ssRandom tiles random-texture.

	//Use Grahm-Schmidt to establish a orthogonal change-of-basis matrix which will reorient offset kernel
	//along occludeeNormal. This also does the random rotation.
	float3 tangent		= normalize(random - occludeeNormal * dot(random, occludeeNormal));
	float3 bitangent	= cross(occludeeNormal, tangent);
	float3x3 tbn		= float3x3(tangent, bitangent, occludeeNormal);

	float occlusion	= 0.0f;
	float radius	= 2.0f; //pass this in a constant buffer
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
			float2((float)threadIDDispatch.x / zNear, (float)threadIDDispatch.y / zFar), //Obs, zNear and zFar not what they seem
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

	occlusion = 1.0f - (occlusion / 14);
	float3 output = float3(occlusion, 0.0f, 0.0f); //saturate(pow(access, 4.0f))
	ssao[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(output, 1.0f);
}

#endif //XKILL_RENDERER_CS_SSAO_HLSL

/*
float4	gNormal = gBufferNormal.SampleLevel(ssNormal, texCoord, 0);
	float	pz		= bufferDepth.SampleLevel(ssDepth, texCoord, 0).x;

	float3 n = mul(float4(gNormal.xyz, 0.0f), view).xyz; //convert normal to view-space

	//Reconstruct view-space position from depth. Observe the normalized coordinates sent to method.
	float3 p = UtilReconstructPositionViewSpace(
		float2(threadIDDispatch.x / viewportWidth, threadIDDispatch.y / viewportHeight), 
		pz, 
		projectionInverse); 

	//Extract random vector and map form [0, 1] -> [-1, +1]
	float3 randVec = 2.0f * bufferRandom.SampleLevel(ssRandom, 4.0f * texCoord, 0.0f).rgb - 1.0f;
	
	float occlusionSum = 0.0f;

	[unroll] for(unsigned int i = 0; i < SAMPLE_COUNT; i++)
	{
		float3 offset = reflect(offsetKernel[i].xyz, randVec);
		float flip = sign(dot(offset, n));
		float3 q = p + flip * occlusionRadius * offset;
		float4 projQ = mul(float4(q, 1.0f), projection); //Obs, proj is not what it seems
		projQ /= projQ.w;

		float rz = bufferDepth.SampleLevel(ssDepth, projQ.xy, 0.0f).a;
		float3 r = UtilReconstructPositionViewSpace(
			float2(projQ.x / viewportWidth, projQ.y / viewportHeight), //float2(threadIDDispatch.x / viewportWidth, threadIDDispatch.y / viewportHeight) 
			rz, 
			projectionInverse); 

		float distZ = p.z - r.z;
		float dp = max(dot(n, normalize(r - p)), 0.0f);
		float occlusion = dp * OcclusionFunction(distZ);

		occlusionSum += occlusion;
	}

	occlusionSum /= SAMPLE_COUNT;

	float access = 1.0f - occlusionSum;
*/