#ifndef XKILL_PHYSICS_FRUSTUMPHYSICSOBJECT
#define XKILL_PHYSICS_FRUSTUMPHYSICSOBJECT

#include "PhysicsObject.h"

class FrustumPhysicsObject
	: public PhysicsObject
{
public:
	FrustumPhysicsObject();
	~FrustumPhysicsObject();
	virtual void onUpdate(float delta);
	bool frustumInit(unsigned int attributeIndex,short);
};

#endif