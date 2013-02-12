#include "PhysicsUtilities.h"

#include <xkill-utilities/Math.h>
#include <btBulletDynamicsCommon.h>

btQuaternion convert(Float4 float4)	
{
	return btQuaternion(float4.x, float4.y, float4.z, float4.w);
}

btVector3 convert(Float3 float3)	
{
	return btVector3(float3.x, float3.y, float3.z);
}

Float3 convert(const btVector3* float3)
{
	return Float3(float3->x(), float3->y(), float3->z());
}

btMatrix3x3 convert(Float4x4 float4x4)
{

	btMatrix3x3 mat = btMatrix3x3(float4x4._11,float4x4._12,float4x4._13,
					   float4x4._21,float4x4._22,float4x4._23,
					   float4x4._31,float4x4._32,float4x4._33);
	return mat;
}

float yawFromQuaternion(const btQuaternion& q)
{
	btQuaternion a = q.normalized();
	float mag = sqrt(a.x()*a.x() + a.z()*a.z());
	if(mag < 0.0001)
	{
		return 2*acos(1.0f);
	}
	return 2*acos(a.x()/mag);
}