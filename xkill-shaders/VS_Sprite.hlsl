
#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

SpriteVSOut VS_Sprite(VSInPosNormTex vsIn)
{
	SpriteVSOut vsOut;

	vsOut.position = mul(spriteMatrix, float4(vsIn.position, 1.0f));
	vsOut.normal = vsIn.normal;
	vsOut.texcoord = vsIn.texcoord;

	return vsOut;
}