#ifndef XKILL_RENDERER_LIGHTDESC_HLSL
#define XKILL_RENDERER_LIGHTDESC_HLSL

struct LightDir
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	
	float3 direction;

	//Padding
	float pad1;
};
struct LightPoint
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3	pos;
	float	range;
	float3	attenuation;

	//Padding
	float pad1;
};
struct LightSpot
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3	pos;
	float	range;
	float3	direction;
	float	spotPow;
	float3	attenuation;

	//Padding
	float pad1;
};

struct SurfaceInfo
{
	float3 position;
	float3 normal;
	float4 diffuse;
	float4 specular;
};

#endif //XKILL_RENDERER_LIGHTDESC_HLSL