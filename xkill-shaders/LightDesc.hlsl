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

	float3	attenuation;
	float	range;
};
struct LightSpot
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3	direction;
	float	range;
	float3	attenuation;
	float	spotPow;
};

//Describes how the various components affect the surface.
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; //w = specpow
};

//struct LightPos
//{
//	float x;
//    float y;
//    float z;
//};

#endif //XKILL_RENDERER_LIGHTDESC_HLSL

/*
struct SurfaceInfo
{
	float3 position;
	float3 normal;
	float4 diffuse;
	float4 specular;
};
*/