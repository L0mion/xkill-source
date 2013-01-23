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

btMatrix3x3 convert(Float4x4 matrix)
{

	btMatrix3x3 mat = btMatrix3x3(matrix._11,matrix._12,matrix._13,
					   matrix._21,matrix._22,matrix._23,
					   matrix._31,matrix._32,matrix._33);
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