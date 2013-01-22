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

float yawFromQuaternion(const btQuaternion& q)
{
	btQuaternion a = q.normalized();
	float mag = sqrt(a.x()*a.x() + a.z()*a.z());
	return 2*acos(a.x()/mag);
}