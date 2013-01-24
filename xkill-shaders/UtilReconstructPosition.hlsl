#ifndef XKILL_RENDERER_UTILRECONSTRUCTPOSITION_HLSL
#define XKILL_RENDERER_UTILRECONSTRUCTPOSITION_HLSL

float3 UtilReconstructPositionViewSpace(float2 texCoord, float z, float4x4 projInverse)
{
	float4 position = float4(
		/*x = */	2.0f	* texCoord.x - 1.0f,
		/*y = */	-2.0f	* texCoord.y + 1.0f,
		/*z = */	-z,
					1.0f);
	position = mul(position, projInverse);

	return position.xyz / position.w;
}

#endif  //XKILL_RENDERER_UTILRECONSTRUCTPOSITION_HLSL