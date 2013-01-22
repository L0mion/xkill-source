#ifndef XKILL_PHYSICS_PHYSICSCOMPONENT2_H
#define XKILL_PHYSICS_PHYSICSCOMPONENT2_H

#include <xkill-utilities/IObserver.h>
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
class CollisionObject;

/*! \defgroup xkill-physics xkill-physics
	Physics Component of XKILL. */

//! Physics Component of XKILL. Uses Bullet Physics. Synchronizes physics attributes with PhysicsObjects.
/*!
\ingroup xkill-physics
*/

static void wrapTickCallback(btDynamicsWorld *world, btScalar timeStep);

class DLL_P PhysicsComponent : public IObserver
{
private:
	btBroadphaseInterface*						broadphase_;
	btDefaultCollisionConfiguration*			collisionConfiguration_;
	btCollisionDispatcher*						dispatcher_;
	btSequentialImpulseConstraintSolver*		solver_;
	btDiscreteDynamicsWorld*					dynamicsWorld_;
	btBulletWorldImporter*						bulletImporter_;

	btAlignedObjectArray<PhysicsObject*>*		physicsObjects_;

	void synchronizeWithAttributes();
	void doCulling();
public:
	PhysicsComponent();
	~PhysicsComponent();
	bool init();
	void onUpdate(float delta);
	void onEvent(Event* e);

	void queueDebugDrawEvent();
	
	void detectedCollisionsDuringStepSimulation(btScalar timeStep);
};

#endif