#ifndef XKILL_RENDERER_CS_SSAO_HLSL
#define XKILL_RENDERER_CS_SSAO_HLSL

#include "constantBuffers.hlsl"

RWTexture2D<float4> ssao : register( u1 );

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
	
	float3 output = float3(1.0f, 0.0f, 0.0f);
	ssao[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(output, 1.0f);
}

#endif //XKILL_RENDERER_CS_SSAO_HLSL