#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <btBulletDynamicsCommon.h>
#include "physicsObject.h"

struct PhysicsAttribute;

class PhysicsObject
{
private:
	btRigidBody* rigidBody_;
protected:
public:
	PhysicsObject();
	~PhysicsObject();
	void Init(PhysicsAttribute* physicsAttribute,
			  btDiscreteDynamicsWorld* dynamicsWorld);
	// Decrepated
	void Init(btCollisionShape* collisionShape,
			  btDefaultMotionState* motionState,
			  btScalar mass,
			  btVector3& localInerta,
			  btDiscreteDynamicsWorld* dynamicsWorld);
	// ---
	void Clean(btDiscreteDynamicsWorld* dynamicsWorld);
	void preStep(PhysicsAttribute* physicsAttribute);
	void postStep(PhysicsAttribute* physicsAttribute);
};

#endif //XKILL_PHYSICS_PHYSICSOBJECT