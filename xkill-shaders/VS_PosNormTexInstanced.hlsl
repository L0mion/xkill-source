#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

VSOutPosNormVTex VS_PosNormTexInstanced(VSInPosNormTexInstanced vsIn)
{
	VSOutPosNormVTex output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.position	= pos;

	float4 normal = float4(vsIn.normal, 0.0f);
	normal = mul(normal, vsIn.world);	//world space
	normal = mul(normal, view);			//view space
	output.normalV = normal.xyz;

	output.texcoord	= vsIn.texcoord;

	return output;
}