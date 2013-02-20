#ifndef XKILL_RENDERER_VS_SCREENQUAD_HLSL
#define XKILL_RENDERER_VS_SCREENQUAD_HLSL

#include "VSOut.hlsl"

struct ScreenQuadVertex
{
	uint	id			: SV_VertexID;
	float4	position	: SV_POSITION;
	float2	texcoord	: TEXCOORD;
};

VSOutPosTex VS_ScreenQuad(ScreenQuadVertex vertex)
{
	VSOutPosTex output;

	if(vertex.id == 0)
	{
		output.position = float4(-1.0, -1.0f, 0.0f, 1.0f);
		output.texcoord = float2(0.0f, 1.0f);
	}
	else if(vertex.id == 1)
	{
		output.position = float4(-1.0, 1.0f, 0.0f, 1.0f);
		output.texcoord = float2(0.0f, 0.0f);
	}
	else if(vertex.id == 2)
	{
		output.position = float4(1.0, -1.0f, 0.0f, 1.0f);
		output.texcoord = float2(1.0f, 1.0f);
	}
	else if(vertex.id == 3)
	{
		output.position = float4(1.0, 1.0f, 0.0f, 1.0f);
		output.texcoord = float2(1.0f, 0.0f);
	}
	else
	{
		output.position = float4(1.0, 1.0f, 1.0f, 1.0f);
		output.texcoord = float2(1.0f, 1.0f);
	}

	return output;
}

#endif //XKILL_RENDERER_VS_SCREENQUAD_HLSL