#ifndef XKILL_RENDERER_LIGHTDIR_HLSL
#define XKILL_RENDERER_LIGHTDIR_HLSL

#include "LightDescDir.hlsl"

void LightDir(
	const float3 toEye,

	const LightDescDir descDir,

	const float4 surfaceAmbient,
	const float4 surfaceDiffuse,
	const float4 surfaceSpecular, //where .w is specular power
	const float3 surfaceNormal,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	//Initialize outputs.
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	const float3 lightVec = -descDir.direction; //Light vector aims opposite the direction the light rays travel.

	//Add ambient term.
	ambient = surfaceAmbient * descDir.ambient;

	const float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		const float3 v = reflect(-lightVec, surfaceNormal);
		const float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceSpecular.w); //specular.w = specpow

		//Add diffuse and specular term.
		diffuse		= diffuseFactor		* surfaceDiffuse	* descDir.diffuse;
		specular	= specularFactor	* surfaceSpecular	* descDir.specular;
	}
}

#endif //XKILL_RENDERER_LIGHTDIR_HLSL