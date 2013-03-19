#ifndef XKILL_PHYSICS_PHYSICSCOMPONENT_H
#define XKILL_PHYSICS_PHYSICSCOMPONENT_H

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/AttributePointer.h>
#include "dllPhysics.h"
template<class T>
class btAlignedObjectArray;
class btBroadphaseInterface;
class btBulletWorldImporter;
class btCollisionDispatcher;
class btCollisionShape;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btSequentialImpulseConstraintSolver;
typedef float btScalar;

class PhysicsObject;
class FrustumPhysicsObject;
class CollisionObject;

class Event_ClosestHitRayCast;
class Event_AllHitsRayCast;
class debugDrawDispatcher;

/*! \defgroup xkill-physics xkill-physics
	Physics Component of XKILL. */

//! Physics Component of XKILL. Uses Bullet Physics. Synchronizes physics attributes with PhysicsObjects.
/*!
\ingroup xkill-physics
*/

static void wrapTickCallback(btDynamicsWorld *world, btScalar timeStep);  //!< wrapper for tickCallback to solve being unable to bind member-function as function pointer issues

struct Attribute_Physics;
class DLL_P PhysicsComponent : public IObserver
{
private:
	btBroadphaseInterface*						broadphase_; //!< Used to cull away collisions early
	btDefaultCollisionConfiguration*			collisionConfiguration_;  //!< Used to setup allocators
	btCollisionDispatcher*						dispatcher_;  //!< Used to deliver collision information to objects
	btSequentialImpulseConstraintSolver*		solver_; //!< Solve impulse equations
	btDiscreteDynamicsWorld*					dynamicsWorld_; //!< Used to handle simulation
	btBulletWorldImporter*						bulletImporter_; //!< Used to import collisionshapes from .bullet files
	debugDrawDispatcher*						debugDrawer_;
	bool										nullProcessExecuting_;

	btAlignedObjectArray<PhysicsObject*>*		physicsObjects_;  //!< List of objects mapping to physics attributes on wich simulation is run
	btAlignedObjectArray<FrustumPhysicsObject*>* frustumPhysicsObjects_;  //!< List of frustums mapping to cameras wich is used to cull physicsobjects

	void synchronizeWithAttributes(AttributePtr<Attribute_Physics> ptr_physics, int physicsAttributeIndex);  //!< Synronize newly added physicsattributes with physicsobjects

	//Refer to CullingComponent.h
	//void updateCulling();  //!< Update frustums, clear culling data, sync frustums with cameras
	//void doCulling(unsigned int frustumAttributeIndex, unsigned int objectAttributeIndex);  //!< Set culling data based on a collision between frustum and physicsobject
	void razeWorld();
public:
	PhysicsComponent();
	~PhysicsComponent();
	bool init();
	void onUpdate(float delta);  //!< loop through all physics objects, synchronizeWithAttributes, update physicsobjects, update culling and step simulation
	void onEvent(Event* e);  //!< handle events for the physicscomponent, mostly deletion/change events of physicsobjects
	void handleEvent_ClosestRayCast(Event_ClosestHitRayCast* event_ClosestRayCast);
	void handleEvent_AllHitsRayCast(Event_AllHitsRayCast* event_AllHitsRayCast);

	void detectedCollisionsDuringStepSimulation(btScalar timeStep);  //!< Receive and filter out collisions in simulation and translate accordingly
};

#endif