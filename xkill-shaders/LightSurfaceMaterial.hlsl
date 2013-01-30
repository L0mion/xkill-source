#ifndef XKILL_RENDERER_LIGHTMATERIAL_HLSL
#define XKILL_RENDERER_LIGHTMATERIAL_HLSL

struct LightSurfaceMaterial
{
	float4 ambient;
	float4 diffuse;
	float4 specular; //w = specpow
};
//Add float4 reflect?

#endif //XKILL_RENDERER_LIGHTMATERIAL_HLSL