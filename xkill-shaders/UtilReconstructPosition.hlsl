#ifndef XKILL_RENDERER_UTILRECONSTRUCTPOSITION_HLSL
#define XKILL_RENDERER_UTILRECONSTRUCTPOSITION_HLSL

//Be weary of me, I don't necessarily work as intended. :S
//float3 UtilReconstructPositionViewSpace(float2 texCoord, float z, float4x4 projInverse)
//{
//	float4 position = float4(
//		/*x = */	2.0f	* texCoord.x - 1.0f,
//		/*y = */	-2.0f	* texCoord.y + 1.0f,
//		/*z = */	-z,
//					1.0f);
//	position = mul(position, projInverse);
//
//	return position.xyz / position.w;
//}

//float3 ComputePositionViewFromZ(float2 positionScreen, float viewSpaceZ, float4x4 proj)
//{
//    float2 screenSpaceRay = float2(
//		positionScreen.x / proj._11,
//		positionScreen.y / proj._22);
//    
//    float3 positionView;
//    positionView.z = viewSpaceZ;
//    // Solve the two projection equations
//    positionView.xy = screenSpaceRay.xy * positionView.z;
//    
//    return positionView;
//}
//
//float3 VSPositionFromDepth(float2 texCoord, float z, float4x4 proj)
//{ 
//    // Get x/w and y/w from the viewport position
//    float x = texCoord.x * 2 - 1;
//    float y = (1 - texCoord.y) * 2 - 1;
//    float4 vProjectedPos = float4(x, y, z, 1.0f);
//    // Transform by the inverse projection matrix
//    float4 vPositionVS = mul(vProjectedPos, proj);  
//    // Divide by w to get the view-space position
//    return vPositionVS.xyz / vPositionVS.w;  
//}
//
//float UnprojectZBufferToViewSpace(float z, float4x4 proj)
//{
//	return (proj._43 / (z - proj._33));
//}

#endif  //XKILL_RENDERER_UTILRECONSTRUCTPOSITION_HLSL