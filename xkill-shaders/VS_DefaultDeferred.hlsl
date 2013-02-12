#include "structs.hlsl"
#include "VSOut.hlsl"

DefaultVSOut VS_DefaultDeferred(quad_vertex vertex)
{
	DefaultVSOut output;

	if(vertex.id == 0)
	{
		output.position = float4(-1.0, -1.0f, 0.0f, 1.0f);
		output.texcoord = float2(0.0f, 1.0f);

		output.positionW = float3(-1.0, -1.0f, 0.0f);
		output.normalW = float3(0.0f, 0.0f, -1.0f);
	}
	else if(vertex.id == 1)
	{
		output.position = float4(-1.0, 1.0f, 0.0f, 1.0f);
		output.texcoord = float2(0.0f, 0.0f);

		output.positionW= float3(-1.0, 1.0f, 0.0f);
		output.normalW = float3(0.0f, 0.0f, -1.0f);
	}
	else if(vertex.id == 2)
	{
		output.position = float4(1.0, -1.0f, 0.0f, 1.0f);
		output.texcoord = float2(1.0f, 1.0f);

		output.positionW= float3(-1.0, 1.0f, 0.0f);
		output.normalW = float3(0.0f, 0.0f, -1.0f);
	}
	else if(vertex.id == 3)
	{
		output.position = float4(1.0, 1.0f, 0.0f, 1.0f);
		output.texcoord = float2(1.0f, 0.0f);

		output.positionW= float3(1.0, 1.0f, 0.0f);
		output.normalW = float3(0.0f, 0.0f, -1.0f);
	}
	else
	{
		output.position = float4(1.0, 1.0f, 1.0f, 1.0f);
		output.texcoord = float2(1.0f, 1.0f);

		output.positionW= float3(1.0, 1.0f, 1.0f);
		output.normalW = float3(0.0f, 0.0f, -1.0f);
	}

	return output;
}