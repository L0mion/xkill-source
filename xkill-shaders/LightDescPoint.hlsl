#ifndef XKILL_RENDERER_LIGHTDESCPOINT_HLSL
#define XKILL_RENDERER_LIGHTDESCPOINT_HLSL

struct LightDescPoint
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3	attenuation;
	float	range;
};

#endif //XKILL_RENDERER_LIGHTDESCPOINT_HLSL