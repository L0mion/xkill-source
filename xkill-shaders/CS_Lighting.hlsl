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

Texture2D gBufferNormal				: register( t0 ); //Register shared in CS_SSAO and CS_Blur_Bilateral
Texture2D gBufferAlbedo				: register( t1 );
Texture2D gBufferMaterial			: register( t2 );
Texture2D bufferGlowHigh			: register( t3 ); //Register shared in PS_DownSample
Texture2D bufferShadowMap			: register( t4 );
Texture2D bufferSSAO				: register( t5 );
Texture2D bufferDepth				: register( t6 ); //Register shared in CS_SSAO and CS_Blur_Bilateral. Also, beware of me. Yarr!
StructuredBuffer<LightDescDir>		lightsDir	: register( t7 );
StructuredBuffer<LightDescPoint>	lightsPoint	: register( t8 );
StructuredBuffer<float3>			lightsPos	: register( t9 );

SamplerState			ss			: register( s0 );
SamplerComparisonState	ssShadow	: register( s1 );

//Shared memory
groupshared uint tileMinDepthInt;
groupshared uint tileMaxDepthInt;
groupshared uint tileLightNum; //Number of lights intersecting tile.

groupshared Frustum tileFrustum;

groupshared float3 lightsPosV[TILE_MAX_LIGHTS];
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
		
		//[unroll] for(uint i = 0; i < TILE_MAX_LIGHTS; i++)
		//{
		//	tileLightIndices[i] = 0;
		//}
	}
	GroupMemoryBarrierWithGroupSync();
	
	//Sample G-Buffers. Data prefetching?
	const float2 texCoord = float2(
		(float)(threadIDDispatch.x + viewportTopX) / (float)screenWidth,
		(float)(threadIDDispatch.y + viewportTopY) / (float)screenHeight);
	
	//Reconstruct view-space position from depth.
	const float	gDepth = bufferDepth.SampleLevel(ss, texCoord, 0).x; //G-BUFFER DEPTH
	const float3 surfacePosV = UtilReconstructPositionViewSpace(
		float2(threadIDDispatch.x / viewportWidth, threadIDDispatch.y / viewportHeight), 
		gDepth, 
		projectionInverse); 
	
	//Get tile depth in view-space.
	InterlockedMin(tileMinDepthInt, asuint(surfacePosV.z)); //Interlocked functions can only be applied onto ints.
	InterlockedMax(tileMaxDepthInt, asuint(surfacePosV.z)); //If one were to check if pixel is 'valid', one would do something akin to this: const bool validPixel = surfacePosV.z >= zNear && surfacePosV.z <= zFar;
	GroupMemoryBarrierWithGroupSync();

	//Fetch tile frustum
	if(threadIDBlockIndex == 0)
	{
		const float tileMinDepthF = asfloat(tileMinDepthInt);
		const float tileMaxDepthF = asfloat(tileMaxDepthInt);

		tileFrustum = ExtractFrustumPlanes(
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
	}
	GroupMemoryBarrierWithGroupSync();
	
	//Cull lights with tile
	const uint numTileThreads = TILE_DIM * TILE_DIM;
	const uint numPasses = (numLightsPoint + numTileThreads - 1) / numTileThreads; //Passes required by tile threads to cover all lights.
	for(uint i = 0; i < numPasses; i++)
	{
		const uint lightIndex = i * numTileThreads + threadIDBlockIndex;
		if(lightIndex >= numLightsPoint)
			break;
		
		//If given light is 'valid'.
		//..and intersects that lights' sphere.
		const float4 lightPosV = mul(float4(lightsPos[lightIndex], 1.0f), view);
		if(IntersectSphere(tileFrustum, lightPosV, lightsPoint[lightIndex].range))
		{
			uint index;
			InterlockedAdd(tileLightNum, 1, index);

			index = min(index, TILE_MAX_LIGHTS);	//Prevent writing outside of allocated array.
			tileLightIndices[index] = lightIndex;	//Last light may be overwritten multiple time if TILE_MAX_LIGHTS is breached.
			lightsPosV[index]		= lightPosV.xyz;
		}
	}
	GroupMemoryBarrierWithGroupSync(); //As this is the last sync - it is after this point we may clip pixel if not valid to make sure we do not evaluate irrelevant pixels.
	
	//Only apply lighting if valid Specular power.
	const float4 gAlbedo	= gBufferAlbedo		.SampleLevel(ss, texCoord, 0); //G-BUFFER ALBEDO
	const float4 gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0); //G-BUFFER MATERIAL
	float4 Ambient	= float4(gAlbedo.xyz, 0.0f);
	float4 Diffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	if(gMaterial.a > 0.0f)
	{
		Ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);

		//Establish view-space normal.
		const float4 gNormal = gBufferNormal.SampleLevel(ss, texCoord, 0); //G-BUFFER NORMAL
		float3 surfaceNormalV = gNormal.xyz;
		surfaceNormalV.x *= 2.0f; surfaceNormalV.x -= 1.0f;
		surfaceNormalV.y *= 2.0f; surfaceNormalV.y -= 1.0f;
		surfaceNormalV.z *= 2.0f; surfaceNormalV.z -= 1.0f;

		const float3 toEyeV	= normalize(float3(0.0f, 0.0f, 0.0f) - surfacePosV);
		
		//Specify surface material.
		LightSurfaceMaterial surfaceMaterial =
		{
			/*Ambient*/		gAlbedo,
			/*Diffuse*/		gAlbedo,
			/*Specular*/	gMaterial
		};

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

			//! OBS - Shadow ought only be applied onto the first directional light, but as we have no more than one, there is no need for branch. - OBS !//
			float4 surfacePosW = mul(float4(surfacePosV, 1.0f), viewInverse); //I'm totally ugly, fix me.
			float4 posH = mul(surfacePosW, shadowMapTransform);
			float shadow = LightShadow(ssShadow, bufferShadowMap, posH);
			diffuse		*= shadow;
			specular	*= shadow;

			Ambient	+= ambient;	
			Diffuse	+= diffuse; 
			Specular += specular;
		}
		const uint numLights = min(tileLightNum, TILE_MAX_LIGHTS); //Clamp tileLightNum as it may be bigger than allowed lights.
		for(i = 0; i < numLights; i++)
		{
			const LightDescPoint descPoint = lightsPoint[tileLightIndices[i]];
			LightPoint(
				toEyeV,
				descPoint,
				lightsPosV[i],
				surfaceMaterial,
				surfaceNormalV,
				surfacePosV,
				ambient, diffuse, specular);	
			Ambient		+= ambient;
			Diffuse		+= diffuse;
			Specular	+= specular;
		}

		//Apply SSAO to ambient lighting only.
		Ambient *= bufferSSAO.SampleLevel(ss, texCoord, 0).x;
	}
	float3 litPixel = Ambient.xyz  + Diffuse.xyz + Specular.xyz;

	//Use additive blending to add glow to the final image using additive blending:
	litPixel = min(litPixel + bufferGlowHigh.SampleLevel(ss, texCoord, 0).xyz, 1.0f);

	//TILING DEMO:
	for(i = 0; i < tileLightNum; i++) //Apply culled point-lights.
	{
		litPixel.g += 0.01;
	}

	output[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(litPixel, 1.0f);
}
