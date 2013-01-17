#include "constantBuffers.hlsl"
#include "structs.hlsl"
#include "utilities.hlsl"

struct PSOut
{
	float4 normal	: SV_TARGET0;
	float4 albedo	: SV_TARGET1;
	float4 material : SV_TARGET2;
};

Texture2D texAlbedo		: register(t0);
Texture2D texNormal		: register(t1);

SamplerState ss : register(s0);

PSOut defaultPS(VSOut pIn)
{
	PSOut output;

	output.normal		= float4(encodeSphereMap(pIn.normalW), 0.0f, pIn.position.z);
	output.albedo		= texAlbedo.SampleLevel(ss, pIn.texcoord, 0);
	
	output.material		= float4(specularTerm, 1.0f);

	return output;
}