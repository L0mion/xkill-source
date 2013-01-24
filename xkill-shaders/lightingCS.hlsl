#include "LightDesc.hlsl"
#include "constantBuffers.hlsl"
#include "lightFunctions.hlsl"
#include "utilities.hlsl"

#define TILE_DIM		16
#define TILE_MAX_LIGHTS	10

//Global memory
RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal						: register( t0 );
Texture2D gBufferAlbedo						: register( t1 );
Texture2D gBufferMaterial					: register( t2 );
Texture2D gBufferDepth						: register( t3 );
StructuredBuffer<LightDir>		lightsDir	: register( t4 );
StructuredBuffer<LightPoint>	lightsPoint	: register( t5 );
StructuredBuffer<LightSpot>		lightsSpot	: register( t6 );
StructuredBuffer<float3>		lightsPos	: register( t7 );

SamplerState ss : register(s0);

//Shared memory
groupshared uint tileMinDepthInt;
groupshared uint tileMaxDepthInt;
groupshared uint tileLightNum;						//Number of lights intersecting tile.
groupshared uint tileLightIndices[TILE_MAX_LIGHTS];	//Indices to lights intersecting tile.

float3 reconstructViewSpacePosition(float2 texCoord, float z)
{
	float4 position = float4(
	/*x = */ 2.0f	* texCoord.x - 1.0f, 
	/*y = */ -2.0f	* texCoord.y + 1.0f, 
	/*z = */ -z, 
	1.0f);

	position = mul(position, projectionInverse);
	return position.xyz / position.w;
}

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
		tileMinDepthInt = 0xFFFFFFFF;
		tileMaxDepthInt = 0.0f;
		tileLightNum	= 0.0f;

		for(uint i = 0; i < TILE_MAX_LIGHTS; i++)
		{
			tileLightIndices[i] = 0;
		}
	}

	//Sample G-Buffers. Data prefetching?
	float2	texCoord	= float2((float)(threadIDDispatch.x + viewportTopX)/(float)screenWidth,(float)(threadIDDispatch.y + viewportTopY)/(float)screenHeight);
	float4	gAlbedo		= gBufferAlbedo		.SampleLevel(ss, texCoord, 0);
	float4	gNormal		= gBufferNormal		.SampleLevel(ss, texCoord, 0);
	float4	gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0);
	float	gDepth		= gBufferDepth		.SampleLevel(ss, texCoord, 0).x;

	float3 surfacePosV = reconstructViewSpacePosition(texCoord, gDepth);
	float3 surfacePosW = mul(float4(surfacePosV, 1.0f), viewInverse).xyz;

	//Get minimum/maximum depth of tile.
	uint pixelDepthInt = asuint(surfacePosV.z);

	GroupMemoryBarrierWithGroupSync();
	InterlockedMin(tileMinDepthInt, pixelDepthInt);
	InterlockedMax(tileMaxDepthInt, pixelDepthInt);
	GroupMemoryBarrierWithGroupSync();
	float tileMinDepthF = asfloat(tileMinDepthInt);
	float tileMaxDepthF = asfloat(tileMaxDepthInt);
	
	//Extract Frustum Planes
	float2 tileScale	= float2(float(screenWidth), float(screenHeight)) * rcp(float(2 * TILE_DIM));
	float2 tileBias		= tileScale - float2(blockID.xy);
	float4 c1 = float4(projection._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
	float4 c2 = float4(0.0f, -projection._22 * tileScale.y, tileBias.y, 0.0f);
	float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);
	float4 frustum[6];
	frustum[0] = c4 - c1; //Sides
	frustum[1] = c4 + c1;
	frustum[2] = c4 - c2;
	frustum[3] = c4 + c2;
	frustum[4] = float4(0.0f, 0.0f, 1.0f,	tileMinDepthF); //Near/Far
	frustum[5] = float4(0.0f, 0.0f, -1.0f,	-tileMaxDepthF);
	[unroll] for(uint i = 0; i < 4; i++) //Normalize frustum sides
	{
		frustum[i] *= rcp(length(frustum[i].xyz));
	}

	//Cull lights with tile
	uint numTileThreads	= TILE_DIM * TILE_DIM;
	uint numPasses		= (numLightsPoint + numTileThreads - 1) / numTileThreads; //Passes required by tile threads to cover all lights.
	for(i = 0; i < numPasses; ++i)
	{
		uint lightIndex = i * numTileThreads + threadIDBlockIndex;

		if(lightIndex < numLightsPoint)
		{
			bool inFrustum = true;
			[unroll] for(uint j = 0; j < 6; j++)
			{
				float d = dot(frustum[j], float4(lightsPos[lightIndex], 1.0f));
				inFrustum = inFrustum && (d >= -lightsPoint[lightIndex].range);
			}

			if(inFrustum && tileLightNum < TILE_MAX_LIGHTS)
			{
				uint index;
				InterlockedAdd(tileLightNum, 1, index);
				tileLightIndices[index] = lightIndex;
			}
		}
	}
	GroupMemoryBarrierWithGroupSync();

	//Apply lighting
	Material surfaceMaterial = 
	{
		/*Ambient*/		gAlbedo,
		/*Diffuse*/		gAlbedo,
		/*Specular*/	float4(0.1f, 0.1f, 0.1f, 1.0f)
	};

	float4 sumAmbient	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 sumDiffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 sumSpecular	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 ambient, diffuse, specular;

	float3 normal = decodeSphereMap(gNormal.xy);
	
	float3 eyePosV	= float3(0.0f, 0.0f, 0.0f);
	float3 eyePosW	= eyePosition;
	float3 toEyeW	= normalize(eyePosW - surfacePosW);
	float3 toEyeV	= normalize(eyePosV - surfacePosV);
	for(i = 0; i < numLightsDir; i++) //Always apply directional lights regardless of culling.
	{
		lightDir(
			surfaceMaterial,
			lightsDir[i],
			normal.xyz,
			toEyeW,
			ambient, diffuse, specular);
	
		sumAmbient	+= ambient;
		sumDiffuse	+= diffuse;
		sumSpecular	+= specular;
	}
	
	for(i = 0; i < numLightsPoint; i++)
	{
		lightPoint(
			surfaceMaterial,
			lightsPoint[i],
			mul(float4(lightsPos[i], 1.0f), viewInverse).xyz, //lightsPos[i], //
			surfacePosW,
			normal.xyz,//mul(float4(normal.xyz, 0.0f), view).xyz, // 
			toEyeW,
			ambient, diffuse, specular);
	
		sumAmbient	+= ambient;
		sumDiffuse	+= diffuse;
		sumSpecular	+= specular;
	}

	//TILING DEMO:
	for(i = 0; i < tileLightNum; i++) //Apply culled point-lights.
	{
		
		sumDiffuse.g += 0.1;
	}

	float4 litSum = sumAmbient + sumDiffuse + sumSpecular;
	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = litSum;
}

//Insert me back into the code when lighting works!
//for(i = 0; i < tileLightNum; i++) //Apply culled point-lights.
//{
//	lightPoint(
//		surfaceMaterial,
//		lightsPoint[pointLightIndex],
//		lightsPos[pointLightIndex],
//		positionV,
//		normal.xyz,
//		toEyeV,
//		ambient, diffuse, specular);
//	
//	sumAmbient	+= ambient;
//	sumDiffuse	+= diffuse;
//	sumSpecular	+= specular;
//	
//	//sumDiffuse.g += 0.1;
//}

//Insert me back into the code plz? :(
//for(i = 0; i < numLightsSpot; i++)
//{
//	LightSpot lightSpot = lightsSpot[i];
//	color += spotLight(
//		surface, 
//		eyePosition, 
//		lightSpot.ambient, 
//		lightSpot.diffuse, 
//		lightSpot.specular, 
//		(float3)lightsPos[numLightsPoint + i], //offset by pointlights
//		lightSpot.range, 
//		lightSpot.direction, 
//		lightSpot.spotPow, 
//		lightSpot.attenuation);
//}

//Old code below:
	//for(i = 0; i < numLightsDir; i++) //Always apply directional lights.
	//{
	//	LightDir lightDir = lightsDir[i];
	//	color += directionalLight(
	//		surface, 
	//		eyePosition, 
	//		lightDir.ambient, 
	//		lightDir.diffuse, 
	//		lightDir.specular, 
	//		lightDir.direction);
	//}

//Apply lighting
	//SurfaceInfo surface = 
	//{
	//	position,
	//	normal.xyz,
	//	albedo,							//diffuse
	//	float4(0.1f, 0.1f, 0.1f, 1.0f)	//specular
	//};