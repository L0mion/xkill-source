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

//Global memory:
Texture2D			toBlur	: register ( t9 );
RWTexture2D<float4>	blurred	: register ( u1 ); 

//Shared memory:
groupshared float4 sharedCache[sharedCacheSize];

#endif //XKILL_RENDERER_CS_BLUR_HLSL