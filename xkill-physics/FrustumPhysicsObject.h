#ifndef XKILL_PHYSICS_FRUSTUMPHYSICSOBJECT
#define XKILL_PHYSICS_FRUSTUMPHYSICSOBJECT

#include "PhysicsObject.h"

class FrustumPhysicsObject
	: public PhysicsObject
{
public:
	FrustumPhysicsObject();
	~FrustumPhysicsObject();
	virtual bool subClassSpecificInitHook();
};

#endif