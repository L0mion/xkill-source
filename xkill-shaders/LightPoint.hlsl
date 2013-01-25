#ifndef XKILL_RENDERER_LIGHTPOINT_HLSL
#define XKILL_RENDERER_LIGHTPOINT_HLSL

#include "LightSurfaceMaterial.hlsl"
#include "LightDescPoint.hlsl"

void LightPoint(
	float3					toEye,

	LightDescPoint			descPoint,
	float3					lightPos,

	LightSurfaceMaterial	surfaceMaterial,
	float3					surfaceNormal,
	float3					surfacePos,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	//Initialize outputs.
	ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = lightPos - surfacePos;

	float d = length(lightVec);
	if(d > descPoint.range)
		return;
	lightVec /= d;

	//Add ambient term.
	ambient = surfaceMaterial.ambient * descPoint.ambient;

	float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, surfaceNormal);
		float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceMaterial.specular.w); //specular.w = specpow
		
		//Add diffuse and specular term.
		diffuse		= diffuseFactor		* surfaceMaterial.diffuse	* descPoint.diffuse;
		specular	= specularFactor	* surfaceMaterial.specular	* descPoint.specular;
	}

	//Attenuate
	float attenuation = 1.0f / dot(descPoint.attenuation, float3(1.0f, d, d * d));
	diffuse		*= attenuation;
	specular	*= attenuation;
}

#endif //XKILL_RENDERER_LIGHTPOINT_HLSL