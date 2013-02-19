
#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

ColorVSOut VS_Color(VSInPosColor colorVSIn)
{
	ColorVSOut colorVSOut;

	colorVSOut.position	= mul(float4(colorVSIn.position, 1.0f), finalMatrix);
	colorVSOut.color	= colorVSIn.color;

	return colorVSOut;
}