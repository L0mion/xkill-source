
#include "constantBuffers.hlsl"
#include "structs.hlsl"

ColorVSOut colorVS(ColorVSIn colorVSIn)
{
	ColorVSOut colorVSOut;

	colorVSOut.position	= mul(finalMatrix, float4(colorVSIn.position, 1.0f));
	colorVSOut.color	= colorVSIn.color;

	return colorVSOut;
}