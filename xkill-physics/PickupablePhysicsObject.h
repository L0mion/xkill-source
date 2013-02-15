#ifndef XKILL_PHYSICS_PICKUPABLEPHYSICSOBJECT
#define XKILL_PHYSICS_PICKUPABLEPHYSICSOBJECT

#include "PhysicsObject.h"
class btDynamicsWorld;

class PickupablePhysicsObject
	: public PhysicsObject
{
public:
	PickupablePhysicsObject();
	~PickupablePhysicsObject();
	virtual bool subClassSpecificInitHook();
	btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	void onUpdate(float delta);
};

#endif