/*! \defgroup xkill-physics xkill-physics
   The component of the xkill-engine which handles physics through wrapping the bullet physics lib.
*/

#ifndef XKILL_PHYSICS_PHYSICSCOMPONENT_H
#define XKILL_PHYSICS_PHYSICSCOMPONENT_H

#include "dllPhysics.h"
#include <xkill-utilities/IObserver.h>

#include <vector>

class PhysicsObject;
class Event;
struct PhysicsAttribute;
struct BoundingAttribute;
struct InputAttribute;

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

template<class T>
class btAlignedObjectArray;

class CollisionShapeManager;


//! Physics Component of XKILL.
/*!
The physics component of XKILL, implements BulletPhysics to simulate the world.
From the Bullet documentation: "The entire physics pipeline computation and its data structures are represented in Bullet by a dynamics world"
"The default dynamics world implementation is the btDiscreteDynamicsWorld."
\ingroup xkill-physics
*/



class DLL_P BulletPhysicsComponent : public IObserver
{
private:
	btBroadphaseInterface* broadphase_; //!< Used to cull collisions
	btDefaultCollisionConfiguration* collisionConfiguration_; //!< Used to setup allocators
	btCollisionDispatcher* dispatcher_; //!< Used to deliver collision information to objects
	btSequentialImpulseConstraintSolver* solver_; //<! Solve impulse equations
	btDiscreteDynamicsWorld* dynamicsWorld_; //<! Used to step simulation
	CollisionShapeManager* collisionShapeManager_;


	std::vector<PhysicsAttribute>* physicsAttributes_; //<! A pointer to the attribute storage containing PhysicsAttributes, external representation of world
	std::vector<BoundingAttribute>* boundingAttributes_; //<! A pointer to the attribute storage containing a copy of bounding volumes for the renderer
	std::vector<InputAttribute>* inputAttributes_; //<! A pointer to the attribute storage containing the input data from the input component

	btAlignedObjectArray<PhysicsObject*>* physicsObjects_; //<! A vector of PhysicsObjects, internal representation of world

public:
	//! Creates a BulletPhysicsComponent, gives it pointers to attribute storages required by the component and sets all pointer to null_ptr
	/*!
	\param physicsAttributes A pointer to the vector containing physicsAttributes
	*/
	BulletPhysicsComponent(std::vector<PhysicsAttribute>* physicsAttributes,
						   std::vector<BoundingAttribute>* boundingAttributes,
						   std::vector<InputAttribute>* inputAttributes);
	//! Deletes all objects created within the component
	~BulletPhysicsComponent();
	//! Initialize the physics component, creates bullet objects and storage for internal representation
	bool init();
	//! Runs every frame and step simulation a number of fixed-length time-steps depending on delta
	/*!
	\param delta The time which the frame should simulate
	*/
	void onUpdate(float delta);
	//! Called when a event is being passed to the component
	/*!
	\param e A pointer to the event that is being passed
	*/
	void onEvent(Event* e);

	//! Creates a rigid body used as a projectile
	void shootSphere();
};

#endif //XKILL_PHYSICS_PHYSICSCOMPONENT_H