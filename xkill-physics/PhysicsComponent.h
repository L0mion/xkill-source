#ifndef XKILL_PHYSICS_PHYSICSCOMPONENT2_H
#define XKILL_PHYSICS_PHYSICSCOMPONENT2_H

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include "dllPhysics.h"
template<class T>
class btAlignedObjectArray;
class btBroadphaseInterface;
class btBulletWorldImporter;
class btCollisionDispatcher;
class btCollisionShape;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;

class PhysicsObject;
class CollisionObject;

class DLL_P PhysicsComponent : public IObserver
{
private:
	btBroadphaseInterface*						broadphase_;
	btDefaultCollisionConfiguration*			collisionConfiguration_;
	btCollisionDispatcher*						dispatcher_;
	btSequentialImpulseConstraintSolver*		solver_;
	btDiscreteDynamicsWorld*					dynamicsWorld_;
	btBulletWorldImporter*						bulletImporter_;

	btAlignedObjectArray<CollisionObject*>*		collisionObjects_;
	btAlignedObjectArray<PhysicsObject*>*		physicsObjects_;
	btAlignedObjectArray<btCollisionShape*>*	collisionShapes_;

	void loadObjects();
	void syncronizeWithAttributes();
	void preStep();
	void postStep();
	void collisionDetection();
	void doCulling();
public:
	PhysicsComponent();
	~PhysicsComponent();
	bool init();
	void onUpdate(float delta);
	void onEvent(Event* e);
};

#endif