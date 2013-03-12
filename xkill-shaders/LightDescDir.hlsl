#ifndef XKILL_RENDERER_LIGHTDESCDIR_HLSL
#define XKILL_RENDERER_LIGHTDESCDIR_HLSL

struct LightDescDir
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	
	float3 direction;

	//Padding
	float pad1; //This bit of padding shouldn't be necessary, but it seemingly is. Get visual errors when not having it.
};

#endif //XKILL_RENDERER_LIGHTDESCDIR_HLSL