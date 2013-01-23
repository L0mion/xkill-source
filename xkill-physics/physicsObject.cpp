#include "PhysicsObject.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "CollisionShapes.h"
#include "MotionState.h"

#include "PhysicsUtilities.h"

AttributeIterator<Attribute_Physics> itrPhysics_;
AttributeIterator<Attribute_Position> itrPosition_PhysicsObject;
AttributeIterator<Attribute_Spatial> itrSpatial_PhysicsObject;

PhysicsObject::PhysicsObject()
	: btRigidBody(-1, nullptr, nullptr)
{
	itrPhysics_ = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial_PhysicsObject = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPosition_PhysicsObject = ATTRIBUTE_MANAGER->position.getIterator();
}

PhysicsObject::~PhysicsObject()
{
	delete getMotionState();
}

btVector3 PhysicsObject::subClassCalculateLocalInertia(btScalar mass)
{
	btVector3 localInertia;
	localInertia.setZero();
	return localInertia;
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
	btScalar mass = static_cast<btScalar>(physicsAttribute->mass);

	//Resolve mass, local inertia of the collision shape, and also the collision shape itself.
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getCollisionShape(physicsAttribute->meshID);
	setCollisionShape(collisionShape);

	btVector3 localInertia = subClassCalculateLocalInertia(mass);
	setMassProps(mass, localInertia); //Set inverse mass and inverse local inertia
	
	if((getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT))
	{
		btTransform world;

		Attribute_Spatial* spatialAttribute = itrSpatial_PhysicsObject.at(itrPhysics_.at(attributeIndex_)->ptr_spatial);
 		Attribute_Position* positionAttribute = itrPosition_PhysicsObject.at(spatialAttribute->ptr_position);
 		world.setOrigin(convert(positionAttribute->position()));
		world.setRotation(convert(spatialAttribute->rotation));
		setWorldTransform(world);  //Static physics objects: transform once
	}
	else
	{
		//Non-static physics objects: let a derived btMotionState handle transforms.
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