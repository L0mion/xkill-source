#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

VSOutPosNormWTex VS_PosNormTexInstanced(VSInPosNormTexInstanced vsIn)
{
	VSOutPosNormWTex output;

	float4 pos = float4(vsIn.position, 1.0f);
	pos = mul(pos, vsIn.world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.position	= pos;

	float4 normal = float4(vsIn.normal, 0.0f);
	normal = mul(normal, vsIn.world);	//world space
	//normal = mul(normal, view);			//view space
	output.normalW = normal.xyz;

	output.texcoord	= vsIn.texcoord;

	return output;
}