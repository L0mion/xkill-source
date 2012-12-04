
#include "structs.hlsl"
#include "constantBuffers.hlsl"
#include "lightFunctions.hlsl"

RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal		: register( t0 );
Texture2D gBufferAlbedo		: register( t1 );

StructuredBuffer<Light> lights : register( t2 );

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

[numthreads(32, 32, 1)]
void defaultCS( uint3 threadID : SV_DispatchThreadID )
{
	float2 texCoord = float2((float)threadID.x/(float)screenWidth,(float)threadID.y/(float)screenHeight);
	float4 albedo	= gBufferAlbedo.SampleLevel(ss, texCoord, 0);
	float3 normal	= gBufferNormal.SampleLevel(ss, texCoord, 0).xyz;
	float3 position = reconstructViewSpacePosition(texCoord);
	
	//Transform position from view space to world space.
	position = mul(float4(position, 1.0f), viewInverse).xyz;
	
	float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	SurfaceInfo surface = {position, normal, albedo, specular};
	
	float3 color = float3(0.0f, 0.0f, 0.0f);
	for(unsigned int i=0; i<numLights; i++)
	{
		if(lights[i].type == LIGHT_TYPE_DIRECTIONAL)
			color += directionalLight(surface, lights[i], eyePosition);
		else if(lights[i].type == LIGHT_TYPE_POINT)
			color += pointLight(surface, lights[i], eyePosition);
		else if(lights[i].type == LIGHT_TYPE_SPOT)
			color += spotLight(surface, lights[i], eyePosition);
	}

	output[threadID.xy] = float4(color, 1.0f);
}






// Transform coordinates from screen space to view space.
//float viewX = (((2.0f*screenX)/screenWidth)-1.0f)/projection._11;
//float viewY = (((-2.0f*screenY)/screenHeight)+1.0f)/projection._22;
//float viewZ = 1.0f;