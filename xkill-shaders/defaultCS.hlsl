
#include "structs.hlsl"
#include "constantBuffers.hlsl"
#include "lightFunctions.hlsl"

RWTexture2D<float4> output : register( u0 );

Texture2D gBufferNormal		: register( t0 );
Texture2D gBufferAlbedo		: register( t1 );

StructuredBuffer<Light> lights : register( t2 );

SamplerState ss : register(s0);

[numthreads(32, 32, 1)]
void defaultCS( uint3 threadID : SV_DispatchThreadID )
{
	float2 texCoord = float2(threadID.x/800.0f, threadID.y/800.0f);
	float4 color = gBufferAlbedo.SampleLevel(ss, texCoord, 0);
	
	//float viewX = (((2.0f*screenX)/screenWidth)-1.0f)/projection_11;
	//float viewY = (((-2.0f*screenY)/screenHeight)+1.0f)/projection_22;
	//float viewZ = 1.0f;



	color *= lights[0].ambient;

	output[threadID.xy] = color;
}