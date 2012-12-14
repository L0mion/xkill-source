#include "physicsUtilities.h"

#include <xkill-utilities/Math.h>
#include <btBulletDynamicsCommon.h>

btVector3 convert(Float4 float4)	
{
	return btVector3(float4.x, float4.y, float4.z);
}