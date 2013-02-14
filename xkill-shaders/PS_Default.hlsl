#include "constantBuffers.hlsl"
#include "VSOut.hlsl"
#include "UtilSphereMapTransform.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
	float4 material : SV_TARGET2;
};

Texture2D texAlbedo		: register(t0);
Texture2D texNormal		: register(t1);

SamplerState ss : register(s0);

PSOut PS_Default(DefaultVSOut pIn)
{
	PSOut output;

	float3 normal = normalize(pIn.normalW);
	normal.x = normal.x * 0.5f + 0.5f;
	normal.y = normal.y * 0.5f + 0.5f;
	normal.z = normal.z * 0.5f + 0.5f;
	
	//Fill normal RTV
	output.normal = float4(normal, 0.0f); 

	pIn.texcoord.y	= 1 - pIn.texcoord.y;
	float3 albedo	= texAlbedo.SampleLevel(ss, pIn.texcoord, 0).xyz;

	//Fill albedo RTV
	output.albedo = float4(albedo, 0.0f);
	
	//Fill material RTV
	output.material		= float4(specularTerm, specularPower);

	return output;
}