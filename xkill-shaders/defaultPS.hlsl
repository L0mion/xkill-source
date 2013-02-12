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

PSOut defaultPS(DefaultVSOut pIn)
{
	PSOut output;

	float3 normal;
	normal.x = pIn.normalW.x * 0.5f + 0.5f;
	normal.y = pIn.normalW.y * 0.5f + 0.5f;
	normal.z = pIn.normalW.z * 0.5f + 0.5f;
	output.normal = float4(normal, 0.0f); //UtilEncodeSphereMap(pIn.normalW);

	pIn.texcoord.y		= 1 - pIn.texcoord.y;
	output.albedo		= texAlbedo.SampleLevel(ss, pIn.texcoord, 0);
	
	output.material		= float4(pIn.positionW, 0.0f);//float4(specularTerm, pIn.positionV.z); //

	return output;
}