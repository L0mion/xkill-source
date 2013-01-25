#ifndef XKILL_RENDERER_TILINGFRUSTUM_HLSL
#define XKILL_RENDERER_TILINGFRUSTUM_HLSL

struct Frustum
{
	float4 _[6];
};

Frustum ExtractFrustumPlanes(
	uint screenWidth,
	uint screenHeight,
	uint tileDim,
	uint2 blockID,
	float4x4 projection,
	float minDepth,
	float maxDepth)
{
	float2 tileScale = float2(float(screenWidth), float(screenHeight)) * rcp(float(2 * tileDim));
	float2 tileBias = tileScale - float2(blockID.xy);
	float4 c1 = float4(projection._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
	float4 c2 = float4(0.0f, -projection._22 * tileScale.y, tileBias.y, 0.0f);
	float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

	Frustum f;
	f._[0] = c4 - c1; //Sides
	f._[1] = c4 + c1;
	f._[2] = c4 - c2;
	f._[3] = c4 + c2;
	f._[4] = float4(0.0f, 0.0f, 1.0f, minDepth); //Near/Far
	f._[5] = float4(0.0f, 0.0f, -1.0f, -maxDepth);
	
	[unroll] for(uint i = 0; i < 4; i++) //Normalize frustum sides
	{
		f._[i] *= rcp(length(f._[i].xyz));
	}

	return f;
}

#endif //XKILL_RENDERER_TILINGFRUSTUM_HLSL