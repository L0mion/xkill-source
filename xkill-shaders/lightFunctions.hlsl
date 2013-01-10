#ifndef XKILL_RENDERER_LIGHTFUNCTIONS_HLSL
#define XKILL_RENDERER_LIGHTFUNCTIONS_HLSL

#include "LightDesc.hlsl"

float3 directionalLight(
	SurfaceInfo	surface, 
	float3		eyePosition, 
	float4		ambient,
	float4		diffuse,
	float4		specular,
	float3		direction)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	float3 lightVec = -direction;
	
	litColor += (surface.diffuse * ambient).xyz;
	
	float diffuseFactor = dot(lightVec, surface.normal);
	
	if(diffuseFactor > 0.0f)
	{
		float	specularPower	= max(surface.specular.a, 1.0f);
		float3	toEye			= normalize(eyePosition - surface.position);
		float3	reflection		= reflect(-lightVec, surface.normal);
		float	specularFactor	= pow(max(dot(reflection, toEye), 0.0f), specularPower);
	
		litColor += (diffuseFactor * surface.diffuse * diffuse).xyz;
		litColor += (specularFactor * surface.specular * specular).xyz;
	}

	return litColor;
}

float3 pointLight(
	SurfaceInfo	surface, 
	float3		eyePosition, 
	float4		ambient,
	float4		diffuse,
	float4		specular,
	float3		pos,
	float		range,
	float3		attenuation)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightVector = pos - surface.position;

	if(length(lightVector) > range)
		return litColor;

	lightVector /= length(lightVector);

	litColor += (surface.diffuse * ambient).xyz;

	float diffuseFactor = dot(lightVector, surface.normal);
	if(diffuseFactor > 0.0f)
	{
		float specularPower = max(surface.specular.a, 1.0f);
		float3 toEye = normalize(eyePosition - surface.position);
		float3 reflection = reflect(-lightVector, surface.normal);
		float specularFactor = pow(max(dot(reflection, toEye), 0.0f), specularPower);

		litColor += (diffuseFactor * surface.diffuse * diffuse).xyz;
		litColor += (specularFactor * surface.specular * specular).xyz;
	}

	return litColor / dot(attenuation, float3(1.0f, length(lightVector), length(lightVector) * length(lightVector)));
}

float3 spotLight(
	SurfaceInfo surface, 
	float3		eyePosition,
	float4		ambient,
	float4		diffuse,
	float4		specular,
	float3		pos,
	float		range,
	float3		direction,
	float		spotPow,
	float3		attenuation)
{
	float3 litColor = pointLight(
		surface,  
		eyePosition, 
		ambient,
		diffuse,
		specular,
		pos,
		range,
		attenuation);

	float3 lightVector = normalize(pos - surface.position);

	float spotlightFactor = pow(max(dot(-lightVector, direction), 0.0f), spotPow);

	return litColor * spotlightFactor;
}

#endif //XKILL_RENDERER_LIGHTFUNCTIONS_HLSL