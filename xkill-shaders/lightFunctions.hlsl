#ifndef XKILL_RENDERER_LIGHTFUNCTIONS_HLSL
#define XKILL_RENDERER_LIGHTFUNCTIONS_HLSL

#include "structs.hlsl"

static const uint LIGHT_TYPE_DIRECTIONAL	= 1;
static const uint LIGHT_TYPE_POINT			= 2;
static const uint LIGHT_TYPE_SPOT			= 3;

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
	
		litColor += (diffuseFactor * surface.diffuse * light.diffuse).xyz;
		litColor += (specularFactor * surface.specular * light.specular).xyz;
	}

	return litColor;
}

float3 pointLight(SurfaceInfo surface, Light light, float3 eyePosition)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightVector = light.position - surface.position;
	float distance = length(lightVector);

	if(distance > light.range)
		return litColor;

	lightVector /= distance;

	litColor += (surface.diffuse * light.ambient).xyz;

	float diffuseFactor = dot(lightVector, surface.normal);
	if(diffuseFactor > 0.0f)
	{
		float specularPower = max(surface.specular.a, 1.0f);
		float3 toEye = normalize(eyePosition - surface.position);
		float3 reflection = reflect(-lightVector, surface.normal);
		float specularFactor = pow(max(dot(reflection, toEye), 0.0f), specularPower);

		litColor += (diffuseFactor * surface.diffuse * light.diffuse).xyz;
		litColor += (specularFactor * surface.specular * light.specular).xyz;
	}

	return litColor / dot(light.attenuation, float3(1.0f, distance, distance*distance));
}

float3 spotLight(SurfaceInfo surface, Light light, float3 eyePosition)
{
	float3 litColor = pointLight(surface, light, eyePosition);

	float3 lightVector = normalize(light.position - surface.position);

	float spotlightFactor = pow(max(dot(-lightVector, light.direction), 0.0f), light.spotPower);

	return litColor * spotlightFactor;
}

#endif //XKILL_RENDERER_LIGHTFUNCTIONS_HLSL