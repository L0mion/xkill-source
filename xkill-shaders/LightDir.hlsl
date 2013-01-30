#ifndef XKILL_RENDERER_LIGHTDIR_HLSL
#define XKILL_RENDERER_LIGHTDIR_HLSL

#include "LightSurfaceMaterial.hlsl"
#include "LightDescDir.hlsl"

void LightDir(
	float3					toEye,

	LightDescDir			descDir,

	LightSurfaceMaterial	surfaceMaterial,
	float3					surfaceNormal,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	//Initialize outputs.
	ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -descDir.direction; //Light vector aims opposite the direction the light rays travel.

	//Add ambient term.
	ambient = surfaceMaterial.ambient * descDir.ambient;

	float diffuseFactor = dot(lightVec, surfaceNormal);

	[flatten] if(diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, surfaceNormal);
		float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceMaterial.specular.w); //specular.w = specpow

		//Add diffuse and specular term.
		diffuse		= diffuseFactor		* surfaceMaterial.diffuse	* descDir.diffuse;
		specular	= specularFactor	* surfaceMaterial.specular	* descDir.specular;
	}
}

#endif //XKILL_RENDERER_LIGHTDIR_HLSL