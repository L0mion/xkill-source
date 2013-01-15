#include "PhysicsObject.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "CollisionShapes.h"
#include "MotionState.h"

#include "PhysicsUtilities.h"

static bool isFirst = true;
AttributeIterator<Attribute_Physics> itrPhysics_2; //if "itrPhysics", compile error: itrPhysics already defined in PhysicsComponent.obj

PhysicsObject::PhysicsObject()
	: btRigidBody(-1, nullptr, nullptr)
{
	yaw_ = 0;
	itrPhysics_2 = ATTRIBUTE_MANAGER->physics.getIterator();
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

	//Get the init data from a physics attribute
	Attribute_Physics* physicsAttribute = itrPhysics_2.at(attributeIndex);

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

	//Bind a motion state to this object. Also set an attribute index to the bound motion state.
	MotionState* customMotionState = new MotionState();
	customMotionState->setAttributeIndex(attributeIndex);
	setMotionState(customMotionState);

	if(!(getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT))
	{
		setAngularVelocity(btVector3(physicsAttribute->angularVelocity.x,
									 physicsAttribute->angularVelocity.y,
									 physicsAttribute->angularVelocity.z));
		setLinearVelocity(btVector3(physicsAttribute->linearVelocity.x,
									 physicsAttribute->linearVelocity.y,
									 physicsAttribute->linearVelocity.z));
		setGravity(btVector3(physicsAttribute->gravity.x,
								physicsAttribute->gravity.y,
								physicsAttribute->gravity.z));
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

void PhysicsObject::onUpdate(float delta)
{
}