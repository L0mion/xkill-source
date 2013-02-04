#include "PhysicsObject.h"

#include <btBulletDynamicsCommon.h>


#include <xkill-utilities/AttributeManager.h>

#include "physicsUtilities.h"
#include <xkill-utilities/Util.h>


#include "CollisionShapes.h"
#include "MotionState.h"

#include "PhysicsUtilities.h"

AttributeIterator<Attribute_Physics> itrPhysics_;
AttributeIterator<Attribute_Position> itrPosition_PhysicsObject;
AttributeIterator<Attribute_Spatial> itrSpatial_PhysicsObject;
static float outOfBoundsIfYIsLowerThanThis;

PhysicsObject::PhysicsObject()
	: btRigidBody(-1, nullptr, nullptr)
{
	itrPhysics_ = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial_PhysicsObject = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPosition_PhysicsObject = ATTRIBUTE_MANAGER->position.getIterator();
	outOfBoundsIfYIsLowerThanThis = -5.0f;
}

PhysicsObject::~PhysicsObject()
{
	delete getMotionState();
}

btVector3 PhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return zeroLocalInertia();
}

btCollisionShape* PhysicsObject::subClassSpecificCollisionShape()
{
	Attribute_Physics* physicsAttribute = itrPhysics_.at(attributeIndex_);
	return CollisionShapes::Instance()->getCollisionShape(physicsAttribute->meshID);
}

bool PhysicsObject::subClassSpecificInitHook()
{
	return true;
}

btVector3 PhysicsObject::localInertiaBasedOnCollisionShapeAndMass(btScalar mass)
{
	btCollisionShape* collisionShape = getCollisionShape();
	btVector3 localInertia;
	collisionShape->calculateLocalInertia(mass, localInertia);
	
	return localInertia;
}

btVector3 PhysicsObject::zeroLocalInertia()
{
	btVector3 localInertia;
	localInertia.setZero();
	return localInertia;
}

bool PhysicsObject::init(unsigned int attributeIndex,unsigned int collisionFilterGroup)
{
	
	if(attributeIndex < 0)
	{
		return false;
	}
	attributeIndex_ = attributeIndex;
	collisionFilterGroup_ = collisionFilterGroup;

	//Get the init data from a physics attribute
	Attribute_Physics* physicsAttribute = itrPhysics_.at(attributeIndex);
	btScalar mass = static_cast<btScalar>(physicsAttribute->mass);

	//Resolve mass, local inertia of the collision shape, and also the collision shape itself.
	btCollisionShape* collisionShape = subClassSpecificCollisionShape();
	setCollisionShape(collisionShape);
	
	btVector3 localInertia = subClassCalculateLocalInertiaHook(mass);
	setMassProps(mass, localInertia); //Set inverse mass and inverse local inertia
	updateInertiaTensor();
	if((getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT))
	{
		btTransform world;

		AttributePtr<Attribute_Spatial> ptr_spatial = itrPhysics_.at(attributeIndex_)->ptr_spatial;
		AttributePtr<Attribute_Position> ptr_position = ptr_spatial->ptr_position;
 		world.setOrigin(convert(ptr_position->position));
		world.setRotation(convert(ptr_spatial->rotation));
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

unsigned int PhysicsObject::getCollisionFilterGroup() const
{
	return collisionFilterGroup_;
}

void PhysicsObject::onUpdate(float delta)
{
	if(getWorldTransform().getOrigin().y() < outOfBoundsIfYIsLowerThanThis)
	{
		handleOutOfBounds();
	}
}

void PhysicsObject::handleOutOfBounds()
{
	btTransform transform;
	btVector3 newPosition = btVector3(0.0f, 10.0f, 0.0f);

	transform = getWorldTransform();
	transform.setOrigin(newPosition);
	setWorldTransform(transform);

	DEBUGPRINT("A physics object was out of bounds. It was moved to a new position " << newPosition.x() << " " << newPosition.y() << " " << newPosition.z());
}