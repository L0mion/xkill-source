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

#include "dllPhysics.h"

#include <vector>

class PhysicsObject;
struct PhysicsAttribute;

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

template<class T>
class btAlignedObjectArray;

class DLL PhysicsComponent
{
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector<PhysicsAttribute>* physicsAttributes;
	unsigned int numPhysicsAttributes;

	btAlignedObjectArray<PhysicsObject*>* physicsObjects;

protected:
public:
	PhysicsComponent();
	~PhysicsComponent();
	bool Init();
	void onUpdate(float delta);
	void onEvent(/*Event* e*/);
};

#endif //XKILL_PHYSICS_PHYSICSCOMPONENT_H