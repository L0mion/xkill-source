#ifndef XKILL_RENDERER_LIGHTFUNCTIONS_HLSL
#define XKILL_RENDERER_LIGHTFUNCTIONS_HLSL

#include "LightDesc.hlsl"

void lightDir(
	Material	surfaceMaterial,
	LightDir	dirLight,
	float3		surfaceNormal,
	float3		toEye,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -dirLight.direction;

	//Establish ambient term:
	ambient = surfaceMaterial.ambient * dirLight.ambient;

	//Establish diffuse and specular term:
	float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, surfaceNormal);
		float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceMaterial.specular.w);

		diffuse		= diffuseFactor		* surfaceMaterial.diffuse	* dirLight.diffuse;
		specular	= specularFactor	* surfaceMaterial.specular	* dirLight.specular; 
	}
}
void lightPoint(
	Material	surfaceMaterial,
	LightPoint	pointLight,
	float3		pointLightPos,
	float3		surfacePos,		//pos of surface in world coordinates
	float3		surfaceNormal,	//normal of surface in world coordinates
	float3		toEye,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Check whether or not light affects surface:
	float3 lightVec = pointLightPos - surfacePos;
	float d = length(lightVec);
	if(d > pointLight.range)
		return;

	lightVec /= d; //Normalize vector

	//Etablish ambient term:
	ambient = surfaceMaterial.ambient * pointLight.ambient;

	//Establish diffuse and specular term:
	float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, surfaceNormal);
		float specularFactor = pow(max(dot(v, toEye), 0.0f), surfaceMaterial.specular.w);

		diffuse		= diffuseFactor		* surfaceMaterial.diffuse	* pointLight.diffuse;
		specular	= specularFactor	* surfaceMaterial.specular	* pointLight.specular; 
	}

	//Attenuate:
	float attenuation = 1.0f / dot(pointLight.attenuation, float3(1.0f, d, d*d));
	diffuse		*= attenuation;
	specular	*= attenuation;
}
void lightSpot(
	Material	material,
	LightSpot	spotLight,
	float3		spotLightPos,
	float3		surfacePos,
	float3		surfaceNormal,
	float3		toEye,
	
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = spotLightPos - surfacePos;
	float d = length(lightVec);
	if(d > spotLight.range)
		return;

	lightVec /= d;

	//Establish ambient term:
	ambient = material.ambient * spotLight.ambient;

	//Establish diffuse and specular term:
	float diffuseFactor = dot(lightVec, surfaceNormal);
	[flatten] if(diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, surfaceNormal);
		float specularFactor = pow(max(dot(v, toEye), 0.0f), material.specular.w);

		diffuse		= diffuseFactor		* material.diffuse	* spotLight.diffuse;
		specular	= specularFactor	* material.specular * spotLight.specular;
	}

	//Spotlight- and attenuation-factor:
	float spot = pow(max(dot(-lightVec, spotLight.direction), 0.0f), spotLight.spotPow);
	float attenuation = spot / dot(spotLight.attenuation, float3(1.0f, d, d*d));
	ambient		*= spot;
	diffuse		*= attenuation;
	specular	*= attenuation;
}

#endif //XKILL_RENDERER_LIGHTFUNCTIONS_HLSL

/* Old lighting funcs:
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
	float3		posEye, 
	float4		ambient,
	float4		diffuse,
	float4		specular,
	float3		posLight,
	float		range,
	float3		attenuation)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightVector = posLight - surface.position;

	if(length(lightVector) > range)
		return litColor;

	lightVector /= length(lightVector);

	litColor += (surface.diffuse * ambient).xyz;

	float diffuseFactor = dot(lightVector, surface.normal);
	if(diffuseFactor > 0.0f)
	{
		float specularPower		= max(surface.specular.a, 1.0f);
		float3 toEye			= normalize(posEye - surface.position);
		float3 reflection		= reflect(-lightVector, surface.normal);
		float specularFactor	= pow(max(dot(reflection, toEye), 0.0f), specularPower);

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

*/