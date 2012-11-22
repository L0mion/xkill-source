RWTexture2D<float4> output : register(u0);

Texture2D gBufferNormal		: register( t0 );
Texture2D gBufferAlbedo		: register( t1 );

cbuffer cbPerFrame : register (b1)
{
	uint screenWidth;
	uint screenHeight;
	uint tileWidth;
	uint tileHeight;
}


SamplerState ss : register(s0);

[numthreads(32, 32, 1)]
void defaultCS( uint3 threadID : SV_DispatchThreadID )
{
	float2 texCoord = float2(threadID.x/800.0f, threadID.y/800.0f);
	float4 color = gBufferAlbedo.SampleLevel(ss, texCoord, 0);
	output[threadID.xy] = color;
}