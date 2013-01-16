
#include "LightDesc.hlsl"
#include "constantBuffers.hlsl"
#include "lightFunctions.hlsl"

#define TILE_DIM		16
#define TILE_MAX_LIGHTS	10

RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal		: register( t0 );
Texture2D gBufferAlbedo		: register( t1 );
Texture2D gBufferMaterial	: register( t2 );

StructuredBuffer<LightDir>		lightsDir	: register( t3 );
StructuredBuffer<LightPoint>	lightsPoint	: register( t4 );
StructuredBuffer<LightSpot>		lightsSpot	: register( t5 );
StructuredBuffer<LightPos>		lightsPos	: register( t6 );

SamplerState ss : register(s0);

//Shared memory
groupshared uint tileMinDepthInt;
groupshared uint tileMaxDepthInt;
groupshared uint tileLightNum;						//Number of lights intersecting tile.
groupshared uint tileLightIndices[TILE_MAX_LIGHTS];	//Indices to lights intersecting tile.

float3 reconstructViewSpacePosition(float2 texCoord)
{
	float4 position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	position.x = 2.0f * texCoord.x - 1.0f;
	position.y = -2.0f * texCoord.y +1.0f;
	position.z = -gBufferNormal.SampleLevel(ss, texCoord, 0).w;

	position = mul(position, projectionInverse);
	position.xyz = position.xyz / position.w;

	return position.xyz;
}

[numthreads(TILE_DIM, TILE_DIM, 1)]
void lightingCS(
	uint3 blockID			: SV_GroupID,
	uint3 threadIDDispatch	: SV_DispatchThreadID,
	uint3 threadIDBlock		: SV_GroupThreadID)
{
	//Sample G-Buffers. Data prefetching?
	float2 texCoord = float2((float)(threadIDDispatch.x + viewportTopX)/(float)screenWidth,(float)(threadIDDispatch.y + viewportTopY)/(float)screenHeight);
	float4 albedo	= gBufferAlbedo.SampleLevel(ss, texCoord, 0);
	float4 normal	= gBufferNormal.SampleLevel(ss, texCoord, 0);
	float4 material = gBufferMaterial.SampleLevel(ss, texCoord, 0);
	float3 position = reconstructViewSpacePosition(texCoord);

	//Get minimum/maximum depth of tile.
	uint pixelDepthInt = asuint(position.z); //Depth currently stored in alpha channel of normal g-buffer.
	tileMinDepthInt = 0xFFFFFFFF;	//These are initializing shared values in a potentially dangerous way. See if there might be a better way of doing this.
	tileMaxDepthInt = 0.0f;			//These are initializing shared values in a potentially dangerous way. See if there might be a better way of doing this.
	tileLightNum	= 0.0f;			//These are initializing shared values in a potentially dangerous way. See if there might be a better way of doing this.
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
	frustum[4] = float4(0.0f, 0.0f, 1.0f, tileMinDepthF); //Near/Far
	frustum[5] = float4(0.0f, 0.0f, -1.0f, -tileMaxDepthF);
	
	for(uint i = 0; i < 4; i++) //Normalize frustum sides
	{
		frustum[i] *= rcp(length(frustum[i].xyz));
	}
	
	for(i = 0; i < numLightsPoint; i++)
	{
		bool inFrustum = true;
		for(uint j = 0; j < 6; j++)
		{
			float d = dot(frustum[j], float4(lightsPos[i], 1.0f));
			inFrustum = inFrustum && (d >= -lightsPoint[i].range);
		}
	
		if(inFrustum)
		{
			uint index;
			InterlockedAdd(tileLightNum, 1, index);
			tileLightIndices[index] = i;
		}
	}
	
	//Apply lighting
	SurfaceInfo surface = 
	{
		position,
		normal.xyz,
		albedo,							//diffuse
		float4(0.1f, 0.1f, 0.1f, 1.0f)	//specular
	};

	float3 color = float3(0.0f, 0.0f, 0.0f);
	for(i = 0; i < numLightsDir; i++)
	{
		color += directionalLight(
			surface, 
			eyePosition, 
			lightsDir[i].ambient, 
			lightsDir[i].diffuse, 
			lightsDir[i].specular, 
			lightsDir[i].direction);
	}
	//for(i = 0; i < tileLightNum; i++)
	//{
	//	//color += pointLight(
	//	//	surface, 
	//	//	eyePosition, 
	//	//	lightsPoint[tileLightIndices[i]].ambient, 
	//	//	lightsPoint[tileLightIndices[i]].diffuse, 
	//	//	lightsPoint[tileLightIndices[i]].specular, 
	//	//	(float3)lightsPos[tileLightIndices[i]],			//lightsPoint[i].pos
	//	//	lightsPoint[tileLightIndices[i]].range, 
	//	//	lightsPoint[tileLightIndices[i]].attenuation); 
	//	//color = float4(1.0f, 1.0f, 1.0f, 1.0f); //color.g = 1.0f;
	//}
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

	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = float4(color, 1.0f);

	//if(tileLightNum > 0)
	//{
	//	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = float4(color, 1.0f);
	//}
	//else
	//{
	//	output[uint2(threadIDDispatch.x + viewportTopX, threadIDDispatch.y + viewportTopY)] = float4(1.0f, 0.0f, 0.0f, 1.0f);
	//}
}

// Transform coordinates from screen space to view space.
//float viewX = (((2.0f*screenX)/screenWidth)-1.0f)/projection._11;
//float viewY = (((-2.0f*screenY)/screenHeight)+1.0f)/projection._22;
//float viewZ = 1.0f;