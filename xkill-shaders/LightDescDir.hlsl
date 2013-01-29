#ifndef XKILL_RENDERER_LIGHTDESCDIR_HLSL
#define XKILL_RENDERER_LIGHTDESCDIR_HLSL

struct LightDescDir
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	
	float3 direction;

	//Padding
	float pad1; //Is this bit of padding necessary?
};

#endif //XKILL_RENDERER_LIGHTDESCDIR_HLSL