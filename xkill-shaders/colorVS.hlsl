
#include "constantBuffers.hlsl"
#include "structs.hlsl"

ColorVSOut colorVS(ColorVSIn colorVSIn)
{
	ColorVSOut colorVSOut;

	colorVSOut.position	= mul(float4(colorVSIn.position, 1.0f), finalMatrix);
	colorVSOut.color	= colorVSIn.color;

	return colorVSOut;
}