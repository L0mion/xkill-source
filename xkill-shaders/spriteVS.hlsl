
#include "constantBuffers.hlsl"
#include "VSOut.hlsl"
#include "VSIn.hlsl"

SpriteVSOut spriteVS(VSInPosNormTex vsIn)
{
	SpriteVSOut vsOut;

	vsOut.position = mul(spriteMatrix, float4(vsIn.position, 1.0f));
	vsOut.normal = vsIn.normal;
	vsOut.texcoord = vsIn.texcoord;

	return vsOut;
}