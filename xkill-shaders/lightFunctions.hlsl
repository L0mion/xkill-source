#ifndef XKILL_RENDERER_LIGHTFUNCTIONS_HLSL
#define XKILL_RENDERER_LIGHTFUNCTIONS_HLSL

#include "structs.hlsl"

float3 directionalLight(SurfaceInfo surface, Light light, float3 eyePosition)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	float3 lightVec = -light.direction;

	litColor += (surface.diffuse * light.ambient).xyz;

	float diffuseFactor = dot(lightVec, surface.normal);

	if(diffuseFactor > 0.0f)
	{
		float	specularPower	= max(surface.specular.a, 1.0f);
		float3	toEye			= normalize(eyePosition - surface.position);
		float3	reflection		= reflect(-lightVec, surface.normal);
		float	specularFactor	= pow(max(dot(reflection, toEye), 0.0f), specularPower);

		litColor += (diffuseFactor * surface.specular * light.specular).xyz;
		litColor += (specularFactor * surface.specular * light.specular).xyz;
	}

	return litColor;
}

#endif //XKILL_RENDERER_LIGHTFUNCTIONS_HLSL