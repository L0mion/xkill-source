#ifndef XKILL_RENDERER_STRUCTS_HLSL
#define XKILL_RENDERER_STRUCTS_HLSL

struct quad_vertex
{
	uint id : SV_VertexID;
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

#endif //XKILL_RENDERER_STRUCTS_HLSL