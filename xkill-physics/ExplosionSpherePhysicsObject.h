#ifndef XKILL_PHYSICS_EXPLOSIONSPHEREPHYSICSOBJECT
#define XKILL_PHYSICS_EXPLOSIONSPHEREPHYSICSOBJECT

#include "PhysicsObject.h"

class ExplosionSpherePhysicsObject
	: public PhysicsObject
{
public:
	ExplosionSpherePhysicsObject();
	~ExplosionSpherePhysicsObject();
	virtual bool subClassSpecificInitHook();
};

#endif