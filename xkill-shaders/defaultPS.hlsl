#include "constantBuffers.hlsl"
#include "structs.hlsl"
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

	output.normal		= float4(UtilEncodeSphereMap(pIn.normalW), 0.0f, 0.0f);
	pIn.texcoord.y = 1-pIn.texcoord.y;
	output.albedo		= texAlbedo.SampleLevel(ss, pIn.texcoord, 0);
	
	output.material		= float4(pIn.positionW, 0.0f);//float4(specularTerm, pIn.positionV.z); //

	return output;
}