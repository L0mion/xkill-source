#include "PhysicsObject.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/Util.h>
#include "CollisionShapes.h"
#include "MotionState.h"
#include "PhysicsUtilities.h"
#include "PhysicsComponent.h"
#include "debugDrawDispatcher.h"

#include <btBulletDynamicsCommon.h>

AttributeIterator<Attribute_Physics> itrPhysics_;
AttributeIterator<Attribute_Position> itrPosition_PhysicsObject;
AttributeIterator<Attribute_Spatial> itrSpatial_PhysicsObject;
static float outOfBoundsIfYIsLowerThanThis;

btDiscreteDynamicsWorld* PhysicsObject::dynamicsWorld_ = NULL;
debugDrawDispatcher* PhysicsObject::debugDrawer_ = NULL;

PhysicsObject::PhysicsObject()
	: btRigidBody(-1, nullptr, nullptr)
{
	itrPhysics_ = ATTRIBUTE_MANAGER->physics.getIterator();
	itrSpatial_PhysicsObject = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPosition_PhysicsObject = ATTRIBUTE_MANAGER->position.getIterator();
	outOfBoundsIfYIsLowerThanThis = -10.0f;
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
	AttributePtr<Attribute_Physics> ptr_physics = itrPhysics_.at(attributeIndex_);
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getCollisionShape(ptr_physics->meshID);
	return collisionShape;
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

btVector3 PhysicsObject::localInertiaBasedOnFirstChildShapeOfCompoundCollisionShapeAndMass(btScalar mass)
{
	btCollisionShape* collisionShape = getCollisionShape();
	btCompoundShape* compoundShape = static_cast<btCompoundShape*>(collisionShape); //check error check here
	btCollisionShape* childCollisionShape = compoundShape->getChildShape(0);

	btVector3 localInertia;
	childCollisionShape->calculateLocalInertia(mass, localInertia);
	
	return localInertia;
}

btVector3 PhysicsObject::zeroLocalInertia()
{
	btVector3 localInertia;
	localInertia.setZero();
	return localInertia;
}

void PhysicsObject::removePhysicsAttributeCorrespondingToThisPhysicsObject()
{
	Entity* ownerEntityOfPhysicsAttribute = itrPhysics_.ownerAt(attributeIndex_);
	int entityOwnerId = ownerEntityOfPhysicsAttribute->getID();
	SEND_EVENT(&Event_RemoveEntity(entityOwnerId));
}

void PhysicsObject::hover(float delta, float hoverHeight)
{
	btVector3 from = getWorldTransform().getOrigin();
	btVector3 to = from - btVector3(0.0f,hoverHeight*2.0f,0.0f);
	btCollisionWorld::ClosestRayResultCallback ray(from,to);
	ray.m_collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::RAY;
	ray.m_collisionFilterMask = XKILL_Enums::PhysicsAttributeType::WORLD;
	dynamicsWorld_->rayTest(from,to,ray); //cast ray from player position straight down
	if(ray.hasHit())
	{
		btVector3 point = from.lerp(to,ray.m_closestHitFraction);
		float length = (point - from).length();
		float something = hoverHeight-length;
		if(something > 0.0f)
		{
			btTransform worldTransform;
			worldTransform = getWorldTransform();
			worldTransform.setOrigin(worldTransform.getOrigin() + btVector3(0.0f,something,0.0f)*delta/0.25f);
			setWorldTransform(worldTransform);

			setLinearVelocity(getLinearVelocity()+btVector3(0.0f,-getLinearVelocity().y(),0.0f));
		}
	}
}

bool PhysicsObject::init(unsigned int attributeIndex, short collisionFilterGroup)
{
	if(attributeIndex < 0)
	{
		return false;
	}
	attributeIndex_ = attributeIndex;
	collisionFilterGroup_ = collisionFilterGroup;

	//Get the init data from a physics attribute
	AttributePtr<Attribute_Physics> ptr_physics = itrPhysics_.at(attributeIndex);
	btScalar mass = static_cast<btScalar>(ptr_physics->mass);

	//Resolve mass, local inertia of the collision shape, and also the collision shape itself.
	btCollisionShape* collisionShape = subClassSpecificCollisionShape();
	if(collisionShape != nullptr)
	{
		setCollisionShape(collisionShape);
	}
	else
	{
		ERROR_MESSAGEBOX("Error in PhysicsObject::init. Expected collision shape pointer unexpectedly set to nullptr. Using default shape instead.");
		setCollisionShape(CollisionShapes::Instance()->getDefaultShape());
	}
	
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

		setAngularVelocity(btVector3(ptr_physics->angularVelocity.x,
										ptr_physics->angularVelocity.y,
										ptr_physics->angularVelocity.z));
		setLinearVelocity(btVector3(ptr_physics->linearVelocity.x,
										ptr_physics->linearVelocity.y,
										ptr_physics->linearVelocity.z));
		//Gravity is set after "addRigidBody" for non-static physics objects
	}

	if(ptr_physics->collisionResponse)
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

short PhysicsObject::getCollisionFilterGroup() const
{
	return collisionFilterGroup_;
}

void PhysicsObject::writeNonSynchronizedPhysicsObjectDataToPhysicsAttribute()
{
	AttributePtr<Attribute_Physics> ptr_physics = itrPhysics_.at(attributeIndex_);

	ptr_physics->angularVelocity = convert(&getAngularVelocity());
	ptr_physics->collisionFilterGroup = getCollisionFilterGroup();
	ptr_physics->collisionResponse = (getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) == 0;
	ptr_physics->gravity = convert(&getGravity());
	ptr_physics->linearVelocity = convert(&getLinearVelocity());
	//ptr_physics->collisionFilterMask = 
	//ptr_physics->mass = physicsObject->getInvMass(); //only mass inverse is stored in physics object
	//ptr_physics->meshID = //not stored in physics object
}
void PhysicsObject::onUpdate(float delta)
{	
	AttributePtr<Attribute_Physics> physicsAttribute = itrPhysics_.at(attributeIndex_);
	Float3 position = physicsAttribute->ptr_spatial->ptr_position->position;
	if(position.y < outOfBoundsIfYIsLowerThanThis)
	if(getWorldTransform().getOrigin().y() < outOfBoundsIfYIsLowerThanThis)
	{
		handleOutOfBounds();
	}

	//if(getWorldTransform().getOrigin().y() < outOfBoundsIfYIsLowerThanThis)
	//{
	//	handleOutOfBounds();
	//}
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