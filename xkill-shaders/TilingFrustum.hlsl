#ifndef XKILL_RENDERER_TILINGFRUSTUM_HLSL
#define XKILL_RENDERER_TILINGFRUSTUM_HLSL

struct Frustum
{
	float4 _[6];
};

Frustum ExtractFrustumPlanes(
	const uint screenWidth,
	const uint screenHeight,
	const uint viewportTopX,
	const uint viewportTopY,
	const uint tileDim,
	const uint2 blockID,
	const float proj11,
	const float proj22,
	const float minZ,
	const float maxZ)
{
    const float2 tileScale = float2(float(screenWidth), float(screenHeight)) * rcp(float(2 * tileDim));
    const float2 tileBias = tileScale - float2(blockID.xy);

    const float4 c1 = float4(proj11 * tileScale.x,	0.0f,					tileBias.x, 0.0f);
    const float4 c2 = float4(0.0f,					-proj22 * tileScale.y,	tileBias.y, 0.0f);
    const float4 c4 = float4(0.0f,					0.0f,					1.0f,		0.0f);

    Frustum f;
    f._[0] = c4 - c1;
    f._[1] = c4 + c1;
    f._[2] = c4 - c2;
    f._[3] = c4 + c2;
    f._[4] = float4(0.0f, 0.0f,  1.0f, -minZ); //Near/Far
    f._[5] = float4(0.0f, 0.0f, -1.0f,  maxZ);
    [unroll] for(uint i = 0; i < 4; ++i)
	{
        f._[i] *= rcp(length(f._[i].xyz));
    }

	return f;
}

bool IntersectSphere(Frustum f, float4 spherePos, float sphereRadius)
{
	bool inFrustum = true;
	[unroll] for(uint j = 0; j < 6; j++)
	{
		float d = dot(
			f._[j],
			spherePos);
		inFrustum = inFrustum && (d >= -sphereRadius);
	}

	return inFrustum;
}

#endif //XKILL_RENDERER_TILINGFRUSTUM_HLSL