#ifndef XKILL_PHYSICS_PLAYERPHYSICSOBJECT
#define XKILL_PHYSICS_PLAYERPHYSICSOBJECT

#include "PhysicsObject.h"

class PlayerPhysicsObject
	: public PhysicsObject
{
public:
	PlayerPhysicsObject();
	~PlayerPhysicsObject();
	bool subClassSpecificInitHook();
	virtual void handleInput();
	virtual void onUpdate(float delta);
};

#endif