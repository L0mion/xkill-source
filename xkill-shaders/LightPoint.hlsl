#ifndef XKILL_RENDERER_LIGHTPOINT_HLSL
#define XKILL_RENDERER_LIGHTPOINT_HLSL

#include "LightDescPoint.hlsl"

void LightPoint(
	const float3 toEye,

	const LightDescPoint descPoint,
	const float3		 lightPos,

	const float4 surfaceAmbient,
	const float4 surfaceDiffuse,
	const float4 surfaceSpecular, //where .w is specular power

	const float3 surfaceNormal,
	const float3 surfacePos,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	//Initialize outputs.
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = lightPos - surfacePos;

	const float d = length(lightVec);
	if(d > descPoint.range)
		return;
	lightVec /= d;

	//Add ambient term.
	ambient = surfaceAmbient * descPoint.ambient;

	const float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, surfaceNormal);
		float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceSpecular.w); //specular.w = specpow
		
		//Add diffuse and specular term.
		diffuse		= diffuseFactor		* surfaceDiffuse	* descPoint.diffuse;
		specular	= specularFactor	* surfaceSpecular	* descPoint.specular;

		//Attenuate
		const float attenuation = 1.0f / dot(descPoint.attenuation, float3(1.0f, d, d * d));
		diffuse		*= attenuation;
		specular	*= attenuation;
	}
}

#endif //XKILL_RENDERER_LIGHTPOINT_HLSL