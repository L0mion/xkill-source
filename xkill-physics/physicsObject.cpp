#include "PhysicsObject.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "CollisionShapes.h"
#include "MotionState.h"

#include "PhysicsUtilities.h"

AttributeIterator<Attribute_Physics> itrPhysics_;

PhysicsObject::PhysicsObject()
	: btRigidBody(-1, nullptr, nullptr)
{
	yaw_ = 0;
	itrPhysics_ = ATTRIBUTE_MANAGER->physics.getIterator();
}

PhysicsObject::~PhysicsObject()
{
	delete getMotionState();
}

bool PhysicsObject::subClassSpecificInitHook()
{
	return true;
}

bool PhysicsObject::init(unsigned int attributeIndex)
{
	if(attributeIndex < 0)
	{
		return false;
	}
	attributeIndex_ = attributeIndex;

	//Get the init data from a physics attribute
	Attribute_Physics* physicsAttribute = itrPhysics_.at(attributeIndex);

	//Resolve mass, local inertia of the collision shape, and also the collision shape itself.
	float mass = static_cast<float>(physicsAttribute->mass);
	btVector3 localInertia;
	localInertia.setZero();
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getCollisionShape(physicsAttribute->meshID);
	if(mass == 0.0f) //calling "setMassProps()" below will set the CF_STATIC_OBJECT flag to true for the btRigidBody if the mass is zero
	{
		collisionShape->calculateLocalInertia(mass, localInertia);
	}
	setMassProps(mass, localInertia); //Set inverse mass and inverse local inertia
	setCollisionShape(collisionShape);

	//CHECK motion state on static objects

	if((getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT))
	{
		//Handle static objects. Set world transform, once.
		btTransform world;
		//setWorldTransform(
	}
	else
	{
		//Bind a motion state to this object. Also set an attribute index to the bound motion state.
		MotionState* customMotionState = new MotionState(attributeIndex);
		setMotionState(customMotionState);

		setAngularVelocity(btVector3(physicsAttribute->angularVelocity.x,
										physicsAttribute->angularVelocity.y,
										physicsAttribute->angularVelocity.z));
		setLinearVelocity(btVector3(physicsAttribute->linearVelocity.x,
										physicsAttribute->linearVelocity.y,
										physicsAttribute->linearVelocity.z));
		//Gravity is set after "addRigidBody" for non-static physics objects
	}

	if(physicsAttribute->collisionResponse)
	{
		setCollisionFlags(getCollisionFlags() & ~CF_NO_CONTACT_RESPONSE); //Activate collision response
	}
	else
	{
		setCollisionFlags(getCollisionFlags() | CF_NO_CONTACT_RESPONSE); //Deactivate collision response
	}
	
	return subClassSpecificInitHook();
}

unsigned int PhysicsObject::getAttributeIndex() const
{
	return attributeIndex_;
}

void PhysicsObject::onUpdate(float delta)
{
}