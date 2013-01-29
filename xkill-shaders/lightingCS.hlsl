#include "LightDescSpot.hlsl"

#include "LightPos.hlsl"
#include "LightDir.hlsl"
#include "LightPoint.hlsl"

#include "TilingFrustum.hlsl"

#include "UtilSphereMapTransform.hlsl"
#include "UtilReconstructPosition.hlsl"
#include "UtilDebug.hlsl"

#include "constantBuffers.hlsl"

#define TILE_DIM		16
#define TILE_MAX_LIGHTS	10

//Global memory
RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal		: register( t0 );
Texture2D gBufferAlbedo		: register( t1 );
Texture2D gBufferMaterial	: register( t2 );
Texture2D gBufferDepth		: register( t3 );
StructuredBuffer<LightDescDir>		lightsDir	: register( t4 );
StructuredBuffer<LightDescPoint>	lightsPoint	: register( t5 );
StructuredBuffer<LightDescSpot>		lightsSpot	: register( t6 );
StructuredBuffer<LightPos>			lightsPos	: register( t7 );

SamplerState ss : register(s0);

//Shared memory
groupshared uint tileMinDepthInt;
groupshared uint tileMaxDepthInt;
groupshared uint tileLightNum; //Number of lights intersecting tile.
groupshared uint tileLightIndices[TILE_MAX_LIGHTS]; //Indices to lights intersecting tile.

[numthreads(TILE_DIM, TILE_DIM, 1)]
void lightingCS(
	uint3	blockID				: SV_GroupID,
	uint	threadIDBlockIndex	: SV_GroupIndex,
	uint3	threadIDDispatch	: SV_DispatchThreadID,
	uint3	threadIDBlock		: SV_GroupThreadID)
{
	//Initialize shared values once per tile
	if(threadIDBlockIndex == 0)
	{
		tileMinDepthInt = 0x7F7FFFFF; //0xFFFFFFFF;
		tileMaxDepthInt = 0.0f;
		tileLightNum	= 0.0f;
		
		for(uint i = 0; i < TILE_MAX_LIGHTS; i++)
		{
			tileLightIndices[i] = 0;
		}
	}
	GroupMemoryBarrierWithGroupSync();
	
	//Sample G-Buffers. Data prefetching?
	float2	texCoord	= float2((float)(threadIDDispatch.x + viewportTopX)/(float)screenWidth,(float)(threadIDDispatch.y + viewportTopY)/(float)screenHeight);
	float4	gAlbedo		= gBufferAlbedo		.SampleLevel(ss, texCoord, 0);
	float4	gNormal		= gBufferNormal		.SampleLevel(ss, texCoord, 0);
	float4	gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0); //At the moment, world space position is stored in Material-buffer.
	float	gDepth		= gBufferDepth		.SampleLevel(ss, texCoord, 0).x;
	
	//Get surface position.
	/*At the moment, world space position is stored in Material-buffer.*/
	float3 surfacePosW = gMaterial.xyz;
	float3 surfacePosV = surfacePosW;//mul(float4(surfacePosW, 1.0f), view).xyz; //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	
	//output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = Ambient + Diffuse + Specular; //float4(tileMinDepthF / zFar, tileMinDepthF / zFar, tileMinDepthF / zFar, 1.0f); //
	
	if(surfacePosV.z > 0.0f)
	{
		output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
}

//uint pixelDepthInt = asuint(surfacePosV.z); //Interlocked functions can only be applied onto ints.
//	
//	InterlockedMin(tileMinDepthInt, pixelDepthInt);
//	InterlockedMax(tileMaxDepthInt, pixelDepthInt);
//	GroupMemoryBarrierWithGroupSync();
//	float tileMinDepthF = asfloat(tileMinDepthInt);
//	float tileMaxDepthF = asfloat(tileMaxDepthInt);
//	
//	Frustum frustum = ExtractFrustumPlanes(
//		screenWidth, 
//		screenHeight, 
//		TILE_DIM, 
//		blockID.xy, 
//		projection._11,
//		projection._22,
//		tileMinDepthF, 
//		tileMaxDepthF);
//	
//	//Cull lights with tile
//	uint numTileThreads = TILE_DIM * TILE_DIM;
//	uint numPasses = (numLightsPoint + numTileThreads - 1) / numTileThreads; //Passes required by tile threads to cover all lights.
//	for(uint i = 0; i < numPasses; ++i)
//	{
//		uint lightIndex = i * numTileThreads + threadIDBlockIndex;
//		
//		if(lightIndex < numLightsPoint)
//		{
//			bool inFrustum = true;
//			[unroll] for(uint j = 0; j < 6; j++)
//			{
//				float d = dot(frustum._[j], mul(float4(lightsPos[lightIndex], 1.0f), view)); //mul(float4(lightsPos[lightIndex], 1.0f)
//				inFrustum = inFrustum && (d >= -lightsPoint[lightIndex].range);
//			}
//			
//			if(inFrustum && tileLightNum < TILE_MAX_LIGHTS)
//			{
//				uint index;
//				InterlockedAdd(tileLightNum, 1, index);
//				tileLightIndices[index] = lightIndex;
//			}
//		}
//	}
//	GroupMemoryBarrierWithGroupSync();
//	
//	float3 normal			= UtilDecodeSphereMap(gNormal.xy);
//	float3 surfaceNormalW	= normalize(normal);
//	float3 surfaceNormalV	= normalize(mul(float4(normal, 0.0f), view).xyz);
//	float3 toEyeW			= normalize(eyePosition - surfacePosW);
//	float3 toEyeV			= normalize(float3(0.0f, 0.0f, 0.0f) - surfacePosV);
//	
//	//Specify surface material.
//	LightSurfaceMaterial surfaceMaterial =
//	{
//		/*Ambient*/		gAlbedo,
//		/*Diffuse*/		gAlbedo,
//		/*Specular*/	float4(0.3f, 0.3f, 0.3f, 1.0f)
//	};
//	
//	//Initialize our resulting and inout-components.
//	float4 Ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 Diffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 Specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 ambient, diffuse, specular;
//	
//	for(i = 0; i < numLightsDir; i++)
//	{
//		LightDescDir descDir = lightsDir[i];
//		LightDir(
//			toEyeV,
//			descDir,
//			surfaceMaterial,
//			surfaceNormalV,
//			ambient, diffuse, specular);
//		Ambient	+= ambient;	
//		Diffuse	+= diffuse; 
//		Specular += specular;
//	}
//	
//	for(i = 0; i < numLightsPoint; i++)
//	{
//		LightDescPoint descPoint = lightsPoint[i];
//		LightPoint(
//			toEyeV,
//			descPoint,
//			mul(float4(lightsPos[i], 1.0f), view).xyz, //
//			surfaceMaterial,
//			surfaceNormalV,
//			surfacePosV,
//			ambient, diffuse, specular);	
//		Ambient		+= ambient;
//		Diffuse		+= diffuse;
//		Specular	+= specular;
//	}
//	
//	//TILING DEMO:
//	//for(i = 0; i < tileLightNum; i++) //Apply culled point-lights.
//	//{
//	//	Diffuse.g += 0.1;
//	//}