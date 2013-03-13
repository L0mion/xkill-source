#ifndef XKILL_RENDERER_UTILBIT_HLSL
#define XKILL_RENDERER_UTILBIT_HLSL

int isVisible(int cull, unsigned int camIndex)
{
	return cull & 1 << camIndex;
}

#endif //XKILL_RENDERER_UTILBIT_HLSL