#include "WorldPhysicsObject.h"
#include <xkill-utilities/Util.h> //DEBUGPRINT

WorldPhysicsObject::WorldPhysicsObject()
	: PhysicsObject()
{
}

WorldPhysicsObject::~WorldPhysicsObject()
{
}

bool WorldPhysicsObject::subClassSpecificInitHook()
{
	return true;
}

btVector3 WorldPhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	setFriction(1.0f);
	return zeroLocalInertia();
}

void WorldPhysicsObject::handleOutOfBounds()
{
	DEBUGPRINT("World physics object out of bounds. Consider moving it to a higher position or change the value of 'outOfBoundsIfYIsLowerThanThis'");
}