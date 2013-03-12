#ifndef XKILL_RENDERER_LIGHTDIR_HLSL
#define XKILL_RENDERER_LIGHTDIR_HLSL

#include "LightDescDir.hlsl"

void LightDir(
	const float3 toEye,

	const float3 lightDir,
	const float4 lightAmbient,
	const float4 lightDiffuse,
	const float4 lightSpecular,

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

	const float3 lightVec = -lightDir; //Light vector aims opposite the direction the light rays travel.

	//Add ambient term.
	ambient = surfaceAmbient * lightAmbient;

	const float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		const float3 v = reflect(-lightVec, surfaceNormal);
		const float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceSpecular.w); //specular.w = specpow

		//Add diffuse and specular term.
		diffuse		= diffuseFactor		* surfaceDiffuse	* lightDiffuse;
		specular	= specularFactor	* surfaceSpecular	* lightSpecular;
	}
}

#endif //XKILL_RENDERER_LIGHTDIR_HLSL