#include "physicsObject.h"

#include <xkill-utilities/AttributeType.h>
#include <vector>
#include <iostream>

#include "physicsUtilities.h"
#include "CollisionShapeManager.h"

PhysicsObject::PhysicsObject(CollisionShapeManager* collisionShapeManager, unsigned int index, unsigned int type) : btRigidBody(1,
																											 new btDefaultMotionState(),
																											 collisionShapeManager->getCollisionShape(0),
																											 btVector3(0,0,0))
{
	index_ = index;
	gravity_.setZero();
	forces_.setZero();
	movement_.setZero();
	yaw_ = 0.0f;
	inertiad = false;
	type_ = type;
}
PhysicsObject::PhysicsObject(btCollisionShape* collisionShape, unsigned int index, unsigned int type) : btRigidBody(0,
																											 new btDefaultMotionState(),
																											 collisionShape,
																											 btVector3(0,0,0))
{
	index_ = index;
	gravity_ = btVector3(0,-10*WorldScaling,0);
	forces_.setZero();
	movement_.setZero();
	yaw_ = 0.0f;
	inertiad = false;
	type_ = type;
}

PhysicsObject::~PhysicsObject()
{
	delete getMotionState();
}

void PhysicsObject::addToWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->addRigidBody(this);
}

void PhysicsObject::removeFromWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->removeRigidBody(this);
}

void PhysicsObject::preStep(CollisionShapeManager* collisionShapeManager,PhysicsAttribute* physicsAttribute)
{
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,
														spatialAttribute,
														physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,
														  positionAttribute,
														  spatialAttribute);
	
	btVector3 localInertia(0,0,0);
	if(getCollisionShape()->getShapeType()==4 && index_ >2)
	{
		//if(!inertiad)
		{
			
			getCollisionShape()->calculateLocalInertia(physicsAttribute->mass,localInertia);
			inertiad = true;
			setRestitution(1.0);
			setRollingFriction(0.01);
			
		}
	}
	setMassProps(physicsAttribute->mass,localInertia);
	
	m_worldTransform.setOrigin(WorldScaling*btVector3(positionAttribute->position.x,
	 												  positionAttribute->position.y,
	 												  positionAttribute->position.z));

	gravity_ = WorldScaling*btVector3(physicsAttribute->gravity.x, physicsAttribute->gravity.y, physicsAttribute->gravity.z);

	if(physicsAttribute->collisionResponse)
	{
		setCollisionFlags(getCollisionFlags() & ~CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		setCollisionFlags(getCollisionFlags() | CF_NO_CONTACT_RESPONSE);
	}

	if(physicsAttribute->isProjectile)
	{
  		m_worldTransform.setRotation(btQuaternion(spatialAttribute->rotation.x, spatialAttribute->rotation.y, spatialAttribute->rotation.z, spatialAttribute->rotation.w));
		setLinearVelocity(btVector3(physicsAttribute->linearVelocity.x,
									physicsAttribute->linearVelocity.y,
									physicsAttribute->linearVelocity.z));
	}
	else
	{
		m_worldTransform.setRotation(btQuaternion(yaw_,0,0));
		movement_.setY(physicsAttribute->linearVelocity.y);
		setLinearVelocity(movement_);
	}
	setAngularVelocity(btVector3(physicsAttribute->angularVelocity.x,
					   physicsAttribute->angularVelocity.y,
					   physicsAttribute->angularVelocity.z));

	if(!physicsAttribute->isExplosionSphere)
	{
		m_collisionShape = collisionShapeManager->getCollisionShape(physicsAttribute->meshID);
	}

	setGravity(gravity_+forces_);
	activate(true);

}

void PhysicsObject::postStep(PhysicsAttribute* physicsAttribute)
{
	//std::cout << "\n" << m_worldTransform.getOrigin().x() << " " << m_worldTransform.getOrigin().y() << m_worldTransform.getOrigin().z();
	SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,
														spatialAttribute,
														physicsAttribute);
	PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,
														  positionAttribute,
														  spatialAttribute);
	btVector3 position = (1.0f/WorldScaling)*m_worldTransform.getOrigin();
	positionAttribute->position.copy(position.m_floats);
	spatialAttribute->rotation.copy(m_worldTransform.getRotation().get128().m128_f32);
	physicsAttribute->linearVelocity.copy(getLinearVelocity().m_floats);
	physicsAttribute->angularVelocity.copy(getAngularVelocity().m_floats);
	forces_.setZero();
}

void PhysicsObject::input(InputAttribute* inputAttribute,float delta)
{
	yaw_ += inputAttribute->rotation.x;
	movement_ = 5*WorldScaling*btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	movement_ = movement_.rotate(btVector3(0,1,0),yaw_);

	inputAttribute->position.x = inputAttribute->position.y = 0;
}

unsigned int PhysicsObject::getIndex() const
{
	return index_;
}

unsigned int PhysicsObject::getType() const
{
	return type_;
}

/*
void PhysicsObject::setCollisionShapeTo(btCollisionShape* collisionShape)
{
	Does not work
	//Scale and set collision shape,
	btCollisionShape* scaleCollisionShape = new btCollisionShape();

	btVector3 vector = btVector3(WorldScaling, WorldScaling, WorldScaling);
	collisionShape->se
	collisionShape->setLocalScaling(vector);
	setCollisionShape(collisionShape);
	
}
*/