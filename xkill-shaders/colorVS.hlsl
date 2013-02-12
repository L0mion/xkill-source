
#include "constantBuffers.hlsl"
#include "VSOut.hlsl"
#include "VSIn.hlsl"

ColorVSOut colorVS(VSInPosColor colorVSIn)
{
	ColorVSOut colorVSOut;

	colorVSOut.position	= mul(float4(colorVSIn.position, 1.0f), finalMatrix);
	colorVSOut.color	= colorVSIn.color;

	return colorVSOut;
}