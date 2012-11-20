/*! \defgroup xkill-physics xkill-physics
   The component of the xkill engine which handles physics through wrapping the bullet physics lib.
   \brief Physics package of XKILL
*/

//! Brief description of documented class.
/*!
A more elaborate description of documented class.
\ingroup DocumentedProject
*/

#ifndef XKILL_PHYSICS_PHYSICSCOMPONENT_H
#define XKILL_PHYSICS_PHYSICSCOMPONENT_H

#include <vector>

#include "dllPhysics.h"
#include "physicsObject.h"

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
class btCollisionShape;
class btDefaultMotionState;
class btDefaultMotionState;
class btRigidBody;
class btRigidBody;

class DLL PhysicsComponent
{
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	PhysicsAttribute* physicsAttributes;
	unsigned int numPhysicsAttribute;


	std::vector<PhysicsObject*> physicsObjects;
	PhysicsObject* ground;
	PhysicsObject* fall;


protected:
public:
	PhysicsComponent();
	~PhysicsComponent();
	bool Init();
	void onUpdate(float delta);
	void onEvent(/*Event* e*/);
};

#endif //XKILL_PHYSICS_PHYSICSCOMPONENT_H