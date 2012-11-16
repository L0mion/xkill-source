
#include "structs.hlsl"

float4 defaultPixelShader(VSOut pIn) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}