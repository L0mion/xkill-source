#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <btBulletDynamicsCommon.h>
#include "physicsObject.h"

struct PhysicsAttribute;
struct InputAttribute;
class CollisionShapeManager;

//! Physics Object
/*!
An object that wraps object specific functionality for rigid bodies.
\ingroup xkill-physics
*/

struct CollisionResult : public btCollisionWorld::ContactResultCallback
{
	bool collision;

	CollisionResult()
	{
		collision = false;
	}
	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1)
	{
		btVector3 ptA = cp.getPositionWorldOnA();
		btVector3 ptB = cp.getPositionWorldOnB();

		collision = true;

		return 0;
	}
};
class PhysicsObject : public btRigidBody
{
private:
	btVector3 gravity_;
	btVector3 forces_;
	btVector3 movement_;
	btScalar yaw_;
protected:
public:

	//! Creates a Physics Object with a rigidbody pointer set to nullptr
	/*!
	\param collisionShapeManager A pointer used to access collision shapes
	*/
	PhysicsObject(CollisionShapeManager* collisionShapeManager);
	//! Deletes all subobjects of the contained rigidbody and the rigidbody itself
	~PhysicsObject();
	//! Initialize rigidbody from physicsAttribute and add object to simulation
	/*! 
	\param physicsAttribute The physics attribute which contain object data
	\param dynamicsWorld The simulation object, used to add the object to the simulation 
	\sa preStep
	*/
	void addToWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	//! Remove the rigidbody from the simulation but do not delete the rigidbody or the object
	/*! 
	\param dynamicsWorld The simulation object, used to the the object from the simulation 
	*/
	void removeFromWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	//! Runs before simulation is stepped to copy data from attribute to rigidbody
	/*! 
	\param physicsAttribute The physics attribute which contain object data
	*/
	void preStep(CollisionShapeManager* collisionShapeManager, PhysicsAttribute* physicsAttribute);
	//! Runs after simulation is stepped to copy data from rigidbody to attribute
	/*! 
	\param physicsAttribute The physics attribute which contain object data
	*/
	void postStep(PhysicsAttribute* physicsAttribute);
	//! Runs after simulation is stepped to copy data from rigidbody to attribute
	/*! 
	\param inputAttribute The attribute containing player controller information
	*/
	void input(InputAttribute* inputAttribute, float delta);
	//! See if an object has been involved in a collision and send an event.
	/*! 
	\param dynamicsWorld The simulation object, used to the the object from the simulation
	\return truthvalue of collision
	*/
	bool contactTest(btDiscreteDynamicsWorld* dynamicsWorld, PhysicsObject& otherPhysicsObject);
};

#endif //XKILL_PHYSICS_PHYSICSOBJECT