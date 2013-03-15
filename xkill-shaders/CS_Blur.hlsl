#ifndef XKILL_RENDERER_CS_BLUR_HLSL
#define XKILL_RENDERER_CS_BLUR_HLSL

/*
Note to self:
Add to weights in blur-kernel to add lighting to scene. (brighter)
Subtract from weights in blur-kernel to remove lighting from the scene. (darker)
*/

#include "constantBuffers.hlsl"

#define blurRadius 5
#define N 256
#define sharedCacheSize (N + 2 * blurRadius)

struct BlurKernel
{
	float _[NUM_BLUR_KERNEL_ELEMENTS_UNCOMPRESSED];
};
const BlurKernel ExtractBlurKernel()
{
	BlurKernel blurKernel;
	[unroll] for(int i = 0; i < NUM_BLUR_KERNEL_ELEMENTS_COMPRESSED; i++)
	{
		blurKernel._[i * 4]		= blurKernelCompressed[i].x;
		blurKernel._[i * 4 + 1]	= blurKernelCompressed[i].y;
		blurKernel._[i * 4 + 2]	= blurKernelCompressed[i].z;
		blurKernel._[i * 4 + 3]	= blurKernelCompressed[i].a;
	}

	return blurKernel;
}

//Global memory:
Texture2D			toBlur	: register ( t9 );
RWTexture2D<float4>	blurred	: register ( u1 ); 

//Shared memory:
groupshared float4 sharedCache[sharedCacheSize];
groupshared BlurKernel blurKernel;

#endif //XKILL_RENDERER_CS_BLUR_HLSL