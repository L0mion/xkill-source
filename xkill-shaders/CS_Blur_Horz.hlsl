#ifndef XKILL_RENDERER_CS_BLUR_HORZ_HLSL
#define XKILL_RENDERER_CS_BLUR_HORZ_HLSL

#include "CS_Blur.hlsl"

/*
Note to self:
Add to weights in blur-kernel to add lighting to scene. (brighter)
Subtract from weights in blur-kernel to remove lighting from the scene. (darker)
*/

#include "constantBuffers.hlsl"

#define blurRadius 5
#define N 256
#define sharedCacheSize (N + 2 * blurRadius)

//Global memory:
Texture2D			toBlur	: register ( t9 );
RWTexture2D<float4>	blurred	: register ( u1 ); 

//Shared memory:
groupshared float4 sharedCache[sharedCacheSize];

[numthreads(N, 1, 1)]
void CS_Blur_Horz(
	int3	threadIDDispatch	: SV_DispatchThreadID,
	int3	threadIDBlock		: SV_GroupThreadID)
{
	//Extract blurring kernel:
	float blurKernel[NUM_BLUR_KERNEL_ELEMENTS_UNCOMPRESSED];
	[unroll] for(int i = 0; i < NUM_BLUR_KERNEL_ELEMENTS_COMPRESSED; i++)
	{
		blurKernel[i * 4]		= blurKernelCompressed[i].x;
		blurKernel[i * 4 + 1]	= blurKernelCompressed[i].y;
		blurKernel[i * 4 + 2]	= blurKernelCompressed[i].z;
		blurKernel[i * 4 + 3]	= blurKernelCompressed[i].a;
	}

	//Load texel into shared memory to reduce memory bandwidth:
	int2 xy = min(
		threadIDDispatch.xy, 
		toBlur.Length.xy - 1); //Clamp out of bound samples that occur at image borders.
	sharedCache[threadIDBlock.x + blurRadius] = toBlur[xy];
	
	//Have some pixels read an additional texel into shared memory to make up for blurRadius:
	if(threadIDBlock.x < blurRadius)
	{
		int x = max(
			threadIDDispatch.x - blurRadius, 
			0); //Clamp out of bound samples that occur at image borders.
		sharedCache[threadIDBlock.x] = toBlur[int2(x, threadIDDispatch.y)];
	}
	if(threadIDBlock.x >= N - blurRadius)
	{
		int x = min(
			threadIDDispatch.x + blurRadius, 
			toBlur.Length.x - 1); //Clamp out of bound samples that occur at image borders.
		sharedCache[threadIDBlock.x + 2 * blurRadius] = toBlur[int2(x, threadIDDispatch.y)];
	}
	GroupMemoryBarrierWithGroupSync();
	
	//Blur pixel:
	float4 blur = float4(0.0f, 0.0f, 0.0f, 1.0f);
	[unroll] for(i = -blurRadius; i <= blurRadius; ++i) //OBS - As HLSL may be strange sometimes, the iterating variable declared in this scope may behave in weird ways. (if a variable of the same name has been declared similarily before. (e.g. unsigned int i)
	{
		int k = threadIDBlock.x + blurRadius + i;
		blur += blurKernel[i + blurRadius] * sharedCache[k];
	}

	blurred[threadIDDispatch.xy] = blur;
}

#endif //XKILL_RENDERER_CS_BLUR_HORZ_HLSL