#include "LightDescPoint.hlsl"
#include "LightDescSpot.hlsl"

#include "LightDir.hlsl"

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
	float4	gMaterial	= gBufferMaterial	.SampleLevel(ss, texCoord, 0);
	float	gDepth		= gBufferDepth		.SampleLevel(ss, texCoord, 0).x;

	float3 surfacePosV = UtilReconstructPositionViewSpace(texCoord, gDepth, projectionInverse);
	float3 surfacePosW = mul(float4(surfacePosV, 1.0f), viewInverse).xyz;

	float3 surfaceNormalW	= normalize(UtilDecodeSphereMap(gNormal.xy));
	float3 toEyeW	= normalize(eyePosition - surfacePosW);

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

	LightDescDir descDir;
	descDir.ambient		= float4(0.05f, 0.05f, 0.05f, 1.0f);
	descDir.diffuse		= float4(0.8f, 0.8f, 0.8f, 1.0f);
	descDir.specular	= float4(0.5f, 0.5f, 0.5f, 1.0f);
	descDir.direction	= float3(0.5f, 0.5f, 0.5f);

	LightDir(
		toEyeW,
		descDir,
		surfaceMaterial,
		surfaceNormalW,
		ambient, diffuse, specular);
	Ambient		+= ambient;
	Diffuse		+= diffuse;
	Specular	+= specular;

	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = Ambient + Diffuse + Specular;
}