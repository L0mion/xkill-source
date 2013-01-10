
#include "LightDesc.hlsl"
#include "constantBuffers.hlsl"
#include "lightFunctions.hlsl"

#define TILE_SIZE 16

RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal		: register( t0 );
Texture2D gBufferAlbedo		: register( t1 );
Texture2D gBufferMaterial	: register( t2 );

StructuredBuffer<LightDir>		lightsDir	: register( t3 );
StructuredBuffer<LightPoint>	lightsPoint	: register( t4 );
StructuredBuffer<LightSpot>		lightsSpot	: register( t5 );

SamplerState ss : register(s0);

float3 reconstructViewSpacePosition(float2 texCoord)
{
	float4 position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	position.x = 2.0f * texCoord.x - 1.0f;
	position.y = -2.0f * texCoord.y +1.0f;
	position.z = gBufferNormal.SampleLevel(ss, texCoord, 0).w;

	position = mul(position, projectionInverse);
	position.xyz = position.xyz / position.w;

	return position.xyz;
}

[numthreads(TILE_SIZE, TILE_SIZE, 1)]
void lightingCS( uint3 threadID : SV_DispatchThreadID )
{
	float2 texCoord = float2((float)(threadID.x + viewportTopX)/(float)screenWidth,(float)(threadID.y + viewportTopY)/(float)screenHeight);
	float4 albedo	= gBufferAlbedo.SampleLevel(ss, texCoord, 0);
	float3 normal	= gBufferNormal.SampleLevel(ss, texCoord, 0).xyz;
	float4 material = gBufferMaterial.SampleLevel(ss, texCoord, 0);
	float3 position = reconstructViewSpacePosition(texCoord);
	
	//Transform position from view space to world space.
	position = mul(float4(position, 1.0f), viewInverse).xyz;
	
	SurfaceInfo surface = 
	{
		position, 
		normal, 
		albedo,							//diffuse
		float4(0.1f, 0.1f, 0.1f, 1.0f)	//specular
	};
	
	float3 color = float3(0.0f, 0.0f, 0.0f);
	for(unsigned int i = 0; i < numLightsDir; i++)
		color += directionalLight(surface, eyePosition, lightsDir[i].ambient, lightsDir[i].diffuse, lightsDir[i].specular, lightsDir[i].direction);
	for(i = 0; i < numLightsPoint; i++)
		color += pointLight(surface, eyePosition, lightsPoint[i].ambient, lightsPoint[i].diffuse, lightsPoint[i].specular, lightsPoint[i].pos, lightsPoint[i].range, lightsPoint[i].attenuation);
	for(i = 0; i < numLightsSpot; i++)
		color += spotLight(surface, eyePosition, lightsSpot[i].ambient, lightsSpot[i].diffuse, lightsSpot[i].specular, lightsSpot[i].pos, lightsSpot[i].range, lightsSpot[i].direction, lightsSpot[i].spotPow, lightsSpot[i].attenuation);

	output[uint2( threadID.x + viewportTopX, threadID.y + viewportTopY)] = float4(color, 1.0f);
}

// Transform coordinates from screen space to view space.
//float viewX = (((2.0f*screenX)/screenWidth)-1.0f)/projection._11;
//float viewY = (((-2.0f*screenY)/screenHeight)+1.0f)/projection._22;
//float viewZ = 1.0f;