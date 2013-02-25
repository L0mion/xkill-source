#include "LightDescSpot.hlsl"

#include "LightPos.hlsl"
#include "LightDir.hlsl"
#include "LightPoint.hlsl"
#include "LightShadow.hlsl"

#include "TilingFrustum.hlsl"

#include "UtilSphereMapTransform.hlsl"
#include "UtilReconstructPosition.hlsl"
#include "UtilDebug.hlsl"

#include "constantBuffers.hlsl"

#define TILE_DIM		16
#define TILE_MAX_LIGHTS	40

//Global memory
RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal				: register( t0 );
Texture2D gBufferAlbedo				: register( t1 );
Texture2D gBufferMaterial			: register( t2 );
Texture2D bufferGlowHigh			: register( t3 ); //Register shared in PS_DownSample
Texture2D bufferShadowMap			: register( t4 );
Texture2D bufferDepth				: register( t5 ); //Hi, please notice me if you intend to sequently add buffers. I can be pretty dangerous if one does not! Yarr.
StructuredBuffer<LightDescDir>		lightsDir	: register( t6 );
StructuredBuffer<LightDescPoint>	lightsPoint	: register( t7 );
StructuredBuffer<LightDescSpot>		lightsSpot	: register( t8 );
StructuredBuffer<float3>			lightsPos	: register( t9 );

SamplerState			ss			: register( s0 );
SamplerComparisonState	ssShadow	: register( s1 );

//Shared memory
groupshared uint tileMinDepthInt;
groupshared uint tileMaxDepthInt;
groupshared uint tileLightNum; //Number of lights intersecting tile.

groupshared uint tileLightIndices[TILE_MAX_LIGHTS]; //Indices to lights intersecting tile.

[numthreads(TILE_DIM, TILE_DIM, 1)]
void CS_Lighting(
	uint3	blockID				: SV_GroupID,
	uint	threadIDBlockIndex	: SV_GroupIndex,
	uint3	threadIDDispatch	: SV_DispatchThreadID,
	uint3	threadIDBlock		: SV_GroupThreadID)
{
	//Initialize shared values once per tile
	if(threadIDBlockIndex == 0)
	{
		tileMinDepthInt = 0xFFFFFFFF;
		tileMaxDepthInt = 0.0f;
		tileLightNum	= 0.0f;
		
		[unroll] for(uint i = 0; i < TILE_MAX_LIGHTS; i++)
		{
			tileLightIndices[i] = 0;
		}
	}
	GroupMemoryBarrierWithGroupSync();
	
	//Sample G-Buffers. Data prefetching?
	float2 texCoord = float2(
		(float)(threadIDDispatch.x + viewportTopX) / (float)screenWidth,
		(float)(threadIDDispatch.y + viewportTopY) / (float)screenHeight);
	float4	gAlbedo		= gBufferAlbedo		.SampleLevel(ss, texCoord, 0);
	float4	gNormal		= gBufferNormal		.SampleLevel(ss, texCoord, 0);
	float4	gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0);
	float	gDepth		= bufferDepth.SampleLevel(ss, texCoord, 0).x; 
	
	//Reconstruct view-space position from depth. Observe the normalized coordinates sent to method.
	float3 surfacePosV = UtilReconstructPositionViewSpace(
		float2(threadIDDispatch.x / viewportWidth, threadIDDispatch.y / viewportHeight), 
		gDepth, 
		projectionInverse); 
	
	//Get tile depth in view-space.
	uint pixelDepthInt = asuint(surfacePosV.z); //Interlocked functions can only be applied onto ints.
	bool validPixel = surfacePosV.z >= zNear && surfacePosV.z <= zFar;
	if(validPixel)
	{
		InterlockedMin(tileMinDepthInt, pixelDepthInt);
		InterlockedMax(tileMaxDepthInt, pixelDepthInt);
	}
	GroupMemoryBarrierWithGroupSync();
	float tileMinDepthF = asfloat(tileMinDepthInt);
	float tileMaxDepthF = asfloat(tileMaxDepthInt);
	
	Frustum frustum = ExtractFrustumPlanes(
		viewportWidth,
		viewportHeight, 
		viewportTopX,
		viewportTopY,
		TILE_DIM, 
		blockID.xy, 
		projection._11,
		projection._22,
		tileMinDepthF, 
		tileMaxDepthF);
	
	//Cull lights with tile
	uint numTileThreads = TILE_DIM * TILE_DIM;
	uint numPasses = (numLightsPoint + numTileThreads - 1) / numTileThreads; //Passes required by tile threads to cover all lights.
	for(uint i = 0; i < numPasses; i++)
	{
		uint lightIndex = i * numTileThreads + threadIDBlockIndex;
		
		//If given light is 'valid'.
		//..and intersects that lights' sphere.
		if(lightIndex < numLightsPoint &&
			IntersectSphere(frustum, mul(float4(lightsPos[lightIndex], 1.0f), view), lightsPoint[lightIndex].range))
		{
			uint index;
			InterlockedAdd(tileLightNum, 1, index);

			index = min(index, TILE_MAX_LIGHTS);	//Prevent writing outside of allocated array.
			tileLightIndices[index] = lightIndex;	//Last light may be overwritten multiple time if TILE_MAX_LIGHTS is breached.
		}
	}
	GroupMemoryBarrierWithGroupSync();

	//Sample depth as quickly as possible to ensure that we do not evualuate irrelevant pixels.
	if(!validPixel)
		return;
	
	float3 normal = gNormal.xyz;
	normal.x *= 2.0f; normal.x -= 1.0f;
	normal.y *= 2.0f; normal.y -= 1.0f;
	normal.z *= 2.0f; normal.z -= 1.0f;
	//float3 normal = decode(gNormal.xy);

	float3 surfaceNormalV	= normalize(mul(float4(normal, 0.0f), view).xyz);
	float3 toEyeV			= normalize(float3(0.0f, 0.0f, 0.0f) - surfacePosV);
	
	//Specify surface material.
	LightSurfaceMaterial surfaceMaterial =
	{
		/*Ambient*/		gAlbedo,
		/*Diffuse*/		gAlbedo,
		/*Specular*/	gMaterial
	};
	
	//Do lighting:
	float4 Ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Diffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 ambient, diffuse, specular;
	for(i = 0; i < numLightsDir; i++)
	{
		LightDescDir descDir = lightsDir[i];
		descDir.direction = mul(float4(descDir.direction, 0.0f), view).xyz;
		LightDir(
			toEyeV,
			descDir,
			surfaceMaterial,
			surfaceNormalV,
			ambient, diffuse, specular);

		if(i == 0)
		{
			//Apply shadow onto first directional light:
			float4 surfacePosW = mul(float4(surfacePosV, 1.0f), viewInverse);
			float4 posH = mul(surfacePosW, shadowMapTransform);
			float shadow = LightShadow(ssShadow, bufferShadowMap, posH);
			diffuse		*= shadow;
			specular	*= shadow;
		}

		Ambient	+= ambient;	
		Diffuse	+= diffuse; 
		Specular += specular;
	}
	uint numLights = min(tileLightNum, TILE_MAX_LIGHTS); //tielLightNum may be bigger than allowed lights.
	for(i = 0; i < numLights; i++)
	{
		LightDescPoint descPoint = lightsPoint[tileLightIndices[i]];
		LightPoint(
			toEyeV,
			descPoint,
			mul(float4(lightsPos[tileLightIndices[i]], 1.0f), view).xyz,
			surfaceMaterial,
			surfaceNormalV,
			surfacePosV,
			ambient, diffuse, specular);	
		Ambient		+= ambient;
		Diffuse		+= diffuse;
		Specular	+= specular;
	}

	//TILING DEMO:
	//for(i = 0; i < tileLightNum; i++) //Apply culled point-lights.
	//{
	//	Diffuse.g += 0.1;
	//}

	float3 litPixel = Ambient.xyz + Diffuse.xyz + Specular.xyz;
	float3 glowPixel = bufferGlowHigh.SampleLevel(ss, texCoord, 0).xyz;
	litPixel = min(litPixel + glowPixel, 1.0f); //additive blending
	output[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(litPixel, 1.0f);
}