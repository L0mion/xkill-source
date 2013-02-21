#ifndef XKILL_PHYSICS_PROPPHYSICSOBJECT
#define XKILL_PHYSICS_PROPPHYSICSOBJECT

#include "physicsObject.h"

class PropPhysicsObject : public PhysicsObject
{
public:
	PropPhysicsObject();
	~PropPhysicsObject();

	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual bool subClassSpecificInitHook();
};

#endif