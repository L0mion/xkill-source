
#include "constantBuffers.hlsl"
#include "structs.hlsl"

SpriteVSOut VS_Sprite(SpriteVSIn vsIn)
{
	SpriteVSOut vsOut;

	vsOut.position = mul(spriteMatrix, float4(vsIn.position, 1.0f));
	vsOut.normal = vsIn.normal;
	vsOut.texcoord = vsIn.texcoord;

	return vsOut;
}