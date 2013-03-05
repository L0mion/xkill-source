#include "PropPhysicsObject.h"
#include <xkill-utilities/AttributeManager.h>
#include "physicsUtilities.h"
#include <Windows.h>

ATTRIBUTES_DECLARE_ALL;
	
PropPhysicsObject::PropPhysicsObject()
{
	ATTRIBUTES_INIT_ALL;
	worldOrigin_ = btVector3(0,0,0);
}

PropPhysicsObject::~PropPhysicsObject()
{

}

btVector3 PropPhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}

bool PropPhysicsObject::subClassSpecificInitHook()
{
	forceActivationState(DISABLE_DEACTIVATION);
	worldOrigin_ = convert(itrPhysics.at(attributeIndex_)->ptr_spatial->ptr_position->position);
	return true;
}

void PropPhysicsObject::handleOutOfBounds()
{
	setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
}