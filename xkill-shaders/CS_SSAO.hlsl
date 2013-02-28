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

	float3 output = float3(saturate(pow(access, 4.0f)), 0.0f, 0.0f);
	ssao[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(output, 1.0f);
}

#endif //XKILL_RENDERER_CS_SSAO_HLSL