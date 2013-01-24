#ifndef XKILL_RENDERER_UTILSPHEREMAPTRANSFORM_HLSL
#define XKILL_RENDERER_UTILSPHEREMAPTRANSFORM_HLSL

float2 UtilEncodeSphereMap(float3 normal)
{
    float oneMinusZ = 1.0f - normal.z;
    float p = sqrt(normal.x * normal.x + normal.y * normal.y + oneMinusZ * oneMinusZ);
    return normal.xy / p * 0.5f + 0.5f;
}

float3 UtilDecodeSphereMap(float2 normalEnc)
{
    float2 tmp = normalEnc - normalEnc * normalEnc;
    float f = tmp.x + tmp.y;
    float m = sqrt(4.0f * f - 1.0f);
    
    float3 n;
    n.xy = m * (normalEnc * 4.0f - 2.0f);
    n.z  = 3.0f - 8.0f * f;
    return n;
}

#endif //XKILL_RENDERER_UTILSPHEREMAPTRANSFORM_HLSL