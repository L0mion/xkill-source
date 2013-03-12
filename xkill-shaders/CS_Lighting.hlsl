#include "LightDescSpot.hlsl"

#include "LightPos.hlsl"
#include "LightDir.hlsl"
#include "LightPoint.hlsl"
#include "LightShadow.hlsl"

#include "TilingFrustum.hlsl"

#include "UtilSphereMapTransform.hlsl"
#include "UtilReconstructPosition.hlsl"

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

groupshared float3 lightsPosV[TILE_MAX_LIGHTS];
groupshared uint tileLightIndices[TILE_MAX_LIGHTS]; //Indices to lights intersecting tile.

[numthreads(TILE_DIM, TILE_DIM, 1)]
void CS_Lighting(
	uint3	blockID				: SV_GroupID,
	uint	threadIDBlockIndex	: SV_GroupIndex,
	uint3	threadIDDispatch	: SV_DispatchThreadID,
	uint3	threadIDBlock		: SV_GroupThreadID)
{
	//Once per tile, do
	if(threadIDBlockIndex == 0)
	{
		//Initialize (those necessary to be initialized) shared values
		tileLightNum = 0;
		tileMinDepthInt = 0xFFFFFFFF;
		tileMaxDepthInt = 0.0f;
	}

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

	const float tileMinDepthF = asfloat(tileMinDepthInt);
	const float tileMaxDepthF = asfloat(tileMaxDepthInt);
	Frustum tileFrustum = ExtractFrustumPlanes(
		viewportWidth,
		viewportHeight, 
		viewportTopX,
		viewportTopY,
		TILE_DIM, 
		blockID.xy, 
		projection._11,
		projection._22,
		tileMinDepthF, 
		tileMaxDepthF); //this could be done by one thread and put in shared memory, or be pre-computed on the cpu for each tile and stored in constant buffers.
	
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
		float3 surfaceNormalV = gNormal.xyz * 2.0f - 1.0f; //uncompress from [0, 1] to [-1, 1]

		const float3 toEyeV	= normalize(float3(0.0f, 0.0f, 0.0f) - surfacePosV);

		//Calculate shadow influence
		const float shadow = LightShadow(
				ssShadow, 
				bufferShadowMap, 
				mul(float4(surfacePosV, 1.0f), shadowMapTransform));

		float4 ambient, diffuse, specular;
		for(i = 0; i < numLightsDir; i++)
		{
			LightDescDir descDir = lightsDir[i];
			descDir.direction = mul(float4(descDir.direction, 0.0f), view).xyz;
			LightDir(
				/*ToEye*/		toEyeV,
				/*Light*/		descDir,
				/*Ambient*/		gAlbedo,
				/*Diffuse*/		gAlbedo,
				/*Specular*/	gMaterial,
				/*Normal*/		surfaceNormalV,
				/*inout*/		ambient, diffuse, specular);

			Ambient		+= ambient;	
			Diffuse		+= diffuse	* shadow; //Shadow ought only be applied onto the first directional light, but as we have no more than one, there is no need for branch.
			Specular	+= specular	* shadow;
		}
		const uint numLights = min(tileLightNum, TILE_MAX_LIGHTS); //Clamp tileLightNum as it may be bigger than allowed lights.
		for(i = 0; i < numLights; i++)
		{
			const LightDescPoint descPoint = lightsPoint[tileLightIndices[i]];
			LightPoint(
				/*ToEye*/			toEyeV,
				/*Light*/			descPoint,
				/*LightPos*/		lightsPosV[i],
				/*Ambient*/			gAlbedo,
				/*Diffuse*/			gAlbedo,
				/*Specular*/		gMaterial,
				/*SurfaceNormal*/	surfaceNormalV,
				/*SurfacePos*/		surfacePosV,
				/*inout*/	ambient, diffuse, specular);	
			Ambient		+= ambient;
			Diffuse		+= diffuse;
			Specular	+= specular;
		}

		//Apply SSAO to ambient lighting only.
		Ambient *= bufferSSAO.SampleLevel(ss, texCoord, 0).x;
	}
	Ambient.xyz = Ambient.xyz  + Diffuse.xyz + Specular.xyz; //Unecessary to take up another register with a temp-var. We just use Ambient to represent the final colour.

	//Use additive blending to add glow to the final image using additive blending:
	Ambient.xyz = min(Ambient.xyz + bufferGlowHigh.SampleLevel(ss, texCoord, 0).xyz, 1.0f);
	output[
		uint2(
			threadIDDispatch.x + viewportTopX, 
			threadIDDispatch.y + viewportTopY)] = 
		float4(Ambient.xyz, 1.0f);
}

//TILING DEMO:
//for(i = 0; i < tileLightNum; i++) //Apply culled point-lights.
//{
//	Ambient.g += 0.1; //0.01
//}