#ifndef XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL
#define XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL

#include "CS_Blur.hlsl"

SamplerState ssDepth	: register( s1 );

//Global
Texture2D gBufferNormal : register( t0	);
Texture2D bufferDepth	: register(	t6	);

//Shared
groupshared float3	sharedNormalV[sharedCacheSize]; //Be sure to use these later
groupshared float	sharedDepth[sharedCacheSize];	

[numthreads(N, 1, 1)]
void CS_Blur_Bilateral_Horz(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID)
{
	//const BlurKernel blurKernel = ExtractBlurKernel();
	//
	////Load texel into shared memory to reduce memory bandwidth:
	//int2 xy = min(
	//	threadIDDispatch.xy, 
	//	toBlur.Length.xy - 1); //Clamp out of bound samples that occur at image borders.
	//sharedCache[threadIDBlock.x + blurRadius] = toBlur[xy];
	//
	////Have some pixels read an additional texel into shared memory to make up for blurRadius:
	//if(threadIDBlock.x < blurRadius)
	//{
	//	int x = max(
	//		threadIDDispatch.x - blurRadius, 
	//		0); //Clamp out of bound samples that occur at image borders.
	//	sharedCache[threadIDBlock.x] = toBlur[int2(x, threadIDDispatch.y)];
	//}
	//if(threadIDBlock.x >= N - blurRadius)
	//{
	//	int x = min(
	//		threadIDDispatch.x + blurRadius, 
	//		toBlur.Length.x - 1); //Clamp out of bound samples that occur at image borders.
	//	sharedCache[threadIDBlock.x + 2 * blurRadius] = toBlur[int2(x, threadIDDispatch.y)];
	//}
	//GroupMemoryBarrierWithGroupSync();
}

#endif //XKILL_RENDERER_CS_BLUR_BILATERAL_HORZ_HLSL