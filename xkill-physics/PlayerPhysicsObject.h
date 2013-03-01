#ifndef XKILL_PHYSICS_PLAYERPHYSICSOBJECT
#define XKILL_PHYSICS_PLAYERPHYSICSOBJECT

#include "PhysicsObject.h"

class PlayerPhysicsObject
	: public PhysicsObject
{
private:
	float yaw_;
public:
	PlayerPhysicsObject();
	~PlayerPhysicsObject();
	bool subClassSpecificInitHook();
	virtual void onUpdate(float delta);
	virtual void handleOutOfBounds();
	virtual void handleInput(float delta);
	virtual void hover(float delta, float hoverHeight);
	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);
	virtual btCollisionShape* subClassSpecificCollisionShape();
};

#endif