#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <btBulletDynamicsCommon.h>
#include "physicsObject.h"

struct PhysicsAttribute;
struct InputAttribute;

//! Physics Object
/*!
An object that wraps object specific functionality for rigid bodies.
\ingroup xkill-physics
*/
class PhysicsObject
{
private:
	btRigidBody* rigidBody_; //!< a pointer to a rigidbody object
	btVector3 forces_;
	btVector3 movement_;
	btScalar yaw_;
protected:
public:

	//! Creates a Physics Object with a rigidbody pointer set to nullptr
	PhysicsObject();


	//! Deletes all subobjects of the contained rigidbody and the rigidbody itself
	~PhysicsObject();
	//! Initialize rigidbody from physicsAttribute and add object to simulation
	/*! 
	\param physicsAttribute The physics attribute which contain object data
	\param dynamicsWorld The simulation object, used to add the object to the simulation 
	\sa preStep
	*/
	void Init(PhysicsAttribute* physicsAttribute,
			  btDiscreteDynamicsWorld* dynamicsWorld);
	//! Remove the rigidbody from the simulation but do not delete the rigidbody or the object
	/*! 
	\param dynamicsWorld The simulation object, used to the the object from the simulation 
	*/
	void Clean(btDiscreteDynamicsWorld* dynamicsWorld);
	//! Runs before simulation is stepped to copy data from attribute to rigidbody
	/*! 
	\param physicsAttribute The physics attribute which contain object data
	*/
	void preStep(PhysicsAttribute* physicsAttribute);
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
};

#endif //XKILL_PHYSICS_PHYSICSOBJECT