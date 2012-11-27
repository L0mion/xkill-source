/*! \defgroup xkill-physics XKILL - Physics
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
#include <xkill-utilities/IObserver.h>

#include <vector>

class PhysicsObject;
class Event;
struct PhysicsAttribute;

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

template<class T>
class btAlignedObjectArray;



class DLL_P BulletPhysicsComponent : public IObserver
{
private:
	btBroadphaseInterface* broadphase_;
	btDefaultCollisionConfiguration* collisionConfiguration_;
	btCollisionDispatcher* dispatcher_;
	btSequentialImpulseConstraintSolver* solver_;
	btDiscreteDynamicsWorld* dynamicsWorld_;

	std::vector<PhysicsAttribute>* physicsAttributes_;
	unsigned int numPhysicsAttributes_;

	btAlignedObjectArray<PhysicsObject*>* physicsObjects_;

	
	//IComponent t;

//protected:
public:
	BulletPhysicsComponent();
	~BulletPhysicsComponent();
	bool Init(std::vector<PhysicsAttribute>* physicsAttributes);
	void onUpdate(float delta);
	void onEvent(Event* e);
};

#endif //XKILL_PHYSICS_PHYSICSCOMPONENT_H