#ifndef XKILL_RENDERER_XKILLDEBUG_HLSL
#define XKILL_RENDERER_XKILLDEBUG_HLSL

bool isBetween(float betweener, float min, float max)
{
	if(betweener > min)
	{
		if(betweener < max)
		{
			return true;
		}
	}
	
	return false;
}
bool is(float iser, float condition)
{
	if(iser == condition)
	{
		return true;
	}
	
	return false;
}

#endif //XKILL_RENDERER_XKILLDEBUG_HLSL