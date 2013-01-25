#include "LightDescSpot.hlsl"

#include "LightDir.hlsl"
#include "LightPoint.hlsl"

#include "UtilSphereMapTransform.hlsl"
#include "UtilReconstructPosition.hlsl"

#include "constantBuffers.hlsl"

#define TILE_DIM		16
#define TILE_MAX_LIGHTS	10

//Global memory
RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal						: register( t0 );
Texture2D gBufferAlbedo						: register( t1 );
Texture2D gBufferMaterial					: register( t2 );
Texture2D gBufferDepth						: register( t3 );
StructuredBuffer<LightDescDir>		lightsDir	: register( t4 );
StructuredBuffer<LightDescPoint>	lightsPoint	: register( t5 );
StructuredBuffer<LightDescSpot>		lightsSpot	: register( t6 );
StructuredBuffer<float3>			lightsPos	: register( t7 );

SamplerState ss : register(s0);

[numthreads(TILE_DIM, TILE_DIM, 1)]
void lightingCS(
	uint3	blockID				: SV_GroupID,
	uint	threadIDBlockIndex	: SV_GroupIndex,
	uint3	threadIDDispatch	: SV_DispatchThreadID,
	uint3	threadIDBlock		: SV_GroupThreadID)
{
	//Sample G-Buffers. Data prefetching?
	float2	texCoord	= float2((float)(threadIDDispatch.x + viewportTopX)/(float)screenWidth,(float)(threadIDDispatch.y + viewportTopY)/(float)screenHeight);
	float4	gAlbedo		= gBufferAlbedo		.SampleLevel(ss, texCoord, 0);
	float4	gNormal		= gBufferNormal		.SampleLevel(ss, texCoord, 0);
	float4	gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0); //At the moment, world space position is stored in Material-buffer.
	float	gDepth		= gBufferDepth		.SampleLevel(ss, texCoord, 0).x;

	//Get surface position.
	/*At the moment, world space position is stored in Material-buffer.*/
	float3 surfacePosW = gMaterial.xyz;
	float3 surfacePosV = mul(float4(gMaterial.xyz, 1.0f), view).xyz;//UtilReconstructPositionViewSpace(texCoord, gMaterial.w, projectionInverse);

	float3 normal = UtilDecodeSphereMap(gNormal.xy);
	float3 surfaceNormalW = normalize(normal);
	float3 surfaceNormalV = normalize(mul(float4(normal, 0.0f), view).xyz);
	float3 toEyeW	= normalize(eyePosition - surfacePosW);
	float3 toEyeV	= normalize(float3(0.0f, 0.0f, 0.0f) - surfacePosV);

	//Specify surface material.
	LightSurfaceMaterial surfaceMaterial =
	{
		/*Ambient*/		gAlbedo,
		/*Diffuse*/		gAlbedo,
		/*Specular*/	float4(0.3f, 0.3f, 0.3f, 1.0f)
	};

	//Initialize our resulting and inout-components.
	float4 Ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Diffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Specular	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 ambient, diffuse, specular;

	for(uint i = 0; i < numLightsDir; i++)
	{
		LightDescDir descDir = lightsDir[i];
		LightDir(
			toEyeV,
			descDir,
			surfaceMaterial,
			surfaceNormalV,
			ambient, diffuse, specular);
		Ambient	+= ambient;	
		Diffuse	+= diffuse; 
		Specular += specular;
	}
	for(i = 0; i < numLightsPoint; i++)
	{
		LightDescPoint descPoint = lightsPoint[i];
		LightPoint(
			toEyeV,
			descPoint,
			mul(float4(lightsPos[i], 1.0f), view),
			surfaceMaterial,
			surfaceNormalV,
			surfacePosV,
			ambient, diffuse, specular);	
		Ambient		+= ambient;
		Diffuse		+= diffuse;
		Specular	+= specular;
	}

	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = Ambient + Diffuse + Specular;
}