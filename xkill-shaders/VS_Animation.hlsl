
#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

DefaultVSOut VS_Animation(VSInPosNormTexTanSkinned vsIn)
{
	DefaultVSOut output;

	float weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	weights[0] = vsIn.weights.x;
	weights[1] = vsIn.weights.y;
	weights[2] = vsIn.weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 position = float3(0.0f, 0.0f, 0.0f);
	float3 normal	= float3(0.0f, 0.0f, 0.0f);

	for(int i=0; i<4; i++)
	{
		//Can at the moment only use one bone to influense each vertex
		position	+= weights[i]*mul(boneTransforms[vsIn.boneIndices[0]], float4(vsIn.position, 1.0f)).xyz;
		normal		+= weights[i]*mul((float3x3)boneTransforms[vsIn.boneIndices[i]], vsIn.normal);
	}


	output.position		= mul(finalMatrix, float4(position, 1.0f));
	output.positionW	= mul(worldMatrix, float4(position, 1.0f)).xyz;
	output.normalW		= mul(worldMatrix, float4(normal, 0.0f)).xyz;
	output.texcoord		= vsIn.texcoord;
	return output;
}