#ifndef XKILL_RENDERER_PS_NORMALMAP_HLSL
#define XKILL_RENDERER_PS_NORMALMAP_HLSL

#include "constantBuffers.hlsl"
#include "VSOut.hlsl"

float3 NormalSampleToWorldSpace(
	float3 normalMapSample,
	float3 unitNormalW,
	float3 tangentW)
{
	//Uncompress each component from [0, 1] to [-1, 1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;
	
		//Build orthonormal basis.
		float3 N = unitNormalW;
		float3 T = normalize(tangentW - dot(tangentW, N) * N);
		float3 B = cross(N, T);

		float3x3 TBN = float3x3(T, B, N);

		//Transform from tangent space to world space.
		float3 bumpedNormalW = mul(normalT, TBN);
		return bumpedNormalW;
}

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

	float3 bumpedNormalW = NormalSampleToWorldSpace(
		texNormal.SampleLevel(ss, pIn.texcoord, 0).xyz,
		normalize(pIn.normalW),
		pIn.tangentW);

	float3 normal = bumpedNormalW; //normalize(pIn.normalW);
	normal.x = normal.x * 0.5f + 0.5f;
	normal.y = normal.y * 0.5f + 0.5f;
	normal.z = normal.z * 0.5f + 0.5f;
	output.normal = float4(normal, 0.0f);

	pIn.texcoord.y		= 1 - pIn.texcoord.y;
	output.albedo		= texAlbedo.SampleLevel(ss, pIn.texcoord, 0);
	
	output.material		= float4(1.0f, 1.0f, 1.0f, 1.0f);

	return output;
}

#endif //XKILL_RENDERER_PS_NORMALMAP_HLSL