#include "ProjectilePhysicsObject.h"

ProjectilePhysicsObject::ProjectilePhysicsObject()
	: PhysicsObject()
{
}

ProjectilePhysicsObject::~ProjectilePhysicsObject()
{
}

bool ProjectilePhysicsObject::subClassSpecificInitHook()
{
	//These values are untweaked. They depend on velocity, size and timeStep.
	setCcdMotionThreshold(1.0f);
	setCcdSweptSphereRadius(0.4f);
	return true;
}