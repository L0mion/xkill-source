#ifndef XKILL_RENDERER_GS_CULL_HLSL
#define XKILL_RENDERER_GS_CULL_HLSL

#include "UtilBit.hlsl"
#include "constantBuffers.hlsl"
#include "VSIn.hlsl"
#include "VSOut.hlsl"

#define GS_CULL_MAX_VERTEX_COUNT 3
[maxvertexcount(GS_CULL_MAX_VERTEX_COUNT)]
void GS_Cull(
	triangle VSInPosNormTexTanInstanced vertices[3],
	inout TriangleStream<VSOutPosNormVTexTanW> visibleVertices)
{
	if(isVisible(vertices[0].cull, camIndex) != 0)
	{
		float4x4 world = vertices[0].world;

		[unroll] for(unsigned int i = 0; i < GS_CULL_MAX_VERTEX_COUNT; i++)
		{
			VSOutPosNormVTexTanW output;

			float4 pos = float4(vertices[i].position, 1.0f);
			pos = mul(pos, world);
			pos = mul(pos, view);
			pos = mul(pos, projection);
			output.position	= pos;
			
			float4 normal = float4(vertices[i].normal, 0.0f);
			normal = mul(normal, world);
			normal = mul(normal, view);
			output.normalV = normal.xyz;
			
			output.tangentW = mul(vertices[i].tangent.xyz, (float3x3)world);
			
			output.texcoord	= vertices[i].texcoord;

			visibleVertices.Append(output);
		}
	}
}

#endif //XKILL_RENDERER_GS_CULL_HLSL