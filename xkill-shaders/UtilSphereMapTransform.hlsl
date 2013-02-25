#ifndef XKILL_RENDERER_UTILSPHEREMAPTRANSFORM_HLSL
#define XKILL_RENDERER_UTILSPHEREMAPTRANSFORM_HLSL

float2 encode(float3 n)
{
	float2 encode = n.xy;
	encode.x *= 0.5f; encode.x += 0.5f;
	encode.y *= 0.5f; encode.y += 0.5f;

	return encode;
}

float3 decode (float2 enc)
{
	float3 decode = float3(enc, 0.0f);
	decode.x *= 2.0f; decode.x -= 1.0f;
	decode.y *= 2.0f; decode.y -= 1.0f;
	decode.z = sqrt(1.0f - decode.x * decode.x - decode.y * decode.y);

	return decode;
}

float2 ReconstructZEncode(float3 normal)
{
	float2 encode = normal.xy;
	encode.x *= 0.5f; encode.x += 0.5f;
	encode.y *= 0.5f; encode.y += 0.5f;
	
	return encode;
}
float3 ReconstructZDecode(float2 encode)
{
	float3 normal = float3(encode, 0.0f);
	normal.x *= normal.x *= 2.0f; normal.x - 1.0f;
	normal.y *= normal.y *= 2.0f; normal.y - 1.0f;
    normal.z = sqrt(1.0f - encode.x * encode.x - encode.y * encode.y);
    
	return normal;
}

#endif //XKILL_RENDERER_UTILSPHEREMAPTRANSFORM_HLSL

/* //Spherical Coordinates:
// Converts a normalized cartesian direction vector
// to spherical coordinates.
float2 CartesianToSpherical(float3 cartesian)
{
  float2 spherical;

  spherical.x = atan2(cartesian.y, cartesian.x) / 3.14159f;
  spherical.y = cartesian.z;

  return spherical * 0.5f + 0.5f;
}

// Converts a spherical coordinate to a normalized
// cartesian direction vector.
float3 SphericalToCartesian(float2 spherical)
{
  float2 sinCosTheta, sinCosPhi;

  spherical = spherical * 2.0f - 1.0f;
  sincos(spherical.x * 3.14159f, sinCosTheta.x, sinCosTheta.y);
  sinCosPhi = float2(sqrt(1.0 - spherical.y * spherical.y), spherical.y);

  return float3(sinCosTheta.y * sinCosPhi.x, sinCosTheta.x * sinCosPhi.x, sinCosPhi.y);    
}
*/

/* //Spheremap-transform:
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
*/