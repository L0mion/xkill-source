#ifndef XKILL_RENDERER_PS_NORMALMAP_HLSL
#define XKILL_RENDERER_PS_NORMALMAP_HLSL

#include "UtilNormalMap.hlsl"
#include "constantBuffers.hlsl"
#include "VSOut.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
	float4 material : SV_TARGET2;
};

Texture2D texAlbedo		: register(t0);
Texture2D texNormal		: register(t1);

SamplerState ss : register(s0);

PSOut PS_NormalMap(VSOutPosNormWTexTanW pIn)
{
	PSOut output;

	pIn.texcoord.y = 1 - pIn.texcoord.y; //wat

	//Sample textures
	float4 normalSample	= texNormal.SampleLevel(ss, pIn.texcoord, 0);
	float4 albedoSample	= texAlbedo.SampleLevel(ss, pIn.texcoord, 0);

	//Fill normal RTV
	float3 normal = NormalSampleToWorldSpace(
		normalSample.xyz,
		normalize(pIn.normalW),
		pIn.tangentW);
	normal.x = normal.x * 0.5f + 0.5f;
	normal.y = normal.y * 0.5f + 0.5f;
	normal.z = normal.z * 0.5f + 0.5f;
	output.normal = float4(normal, 0.0f);

	//Fill albedo RTV
	output.albedo = float4(albedoSample.xyz, 0.0f);
	
	//Fill material RTV
	output.material	= float4(specularTerm, 0.0f);

	//Fill glow RTV
	//output.glowHigh = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//if(normalSample.w > 0.0f)
	//{
	//	float3 glowColor = albedoSample.xyz * normalSample.w;
	//	output.glowHigh = float4(glowColor, 1.0f);
	//}

	return output;
}

#endif //XKILL_RENDERER_PS_NORMALMAP_HLSL