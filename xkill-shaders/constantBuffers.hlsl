
cbuffer cbPerFrame : register (b0)
{
	float4x4 worldViewProj;
}

cbuffer cbPerFrame : register (b1)
{
	uint screenWidth;
	uint screenHeight;
	uint tileWidth;
	uint tileHeight;
}