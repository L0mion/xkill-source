#ifndef XKILL_PHYSICS_PICKUPABLEPHYSICSOBJECT
#define XKILL_PHYSICS_PICKUPABLEPHYSICSOBJECT

#include "PhysicsObject.h"
class btDynamicsWorld;

class PickupablePhysicsObject
	: public PhysicsObject
{
private:
	bool removeAfterOnUpdate;
public:
	PickupablePhysicsObject();
	~PickupablePhysicsObject();
	virtual bool subClassSpecificInitHook();
	btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual void handleOutOfBounds();
	void onUpdate(float delta);
};

#endif