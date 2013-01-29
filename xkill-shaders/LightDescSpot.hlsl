#ifndef XKILL_RENDERER_LIGHTDESCSPOT_HLSL
#define XKILL_RENDERER_LIGHTDESCSPOT_HLSL

struct LightDescSpot
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3	direction;
	float	range;
	float3	attenuation;
	float	spotPow;
};

#endif //XKILL_RENDERER_LIGHTDESCSPOT_HLSL