#include "physicsObject.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "collisionShapes.h"
#include "motionState.h"

#include "physicsUtilities.h"


ATTRIBUTES_DECLARE_ALL;
static bool isFirst = true;



PhysicsObject::PhysicsObject(unsigned int attributeIndex) : btRigidBody(0, new MotionState(attributeIndex), nullptr)
{
	yaw_ = 0;
}

PhysicsObject::~PhysicsObject()
{
	delete getMotionState();
}
void PhysicsObject::init(unsigned int attributeIndex)
{
	if(isFirst)
	{
		ATTRIBUTES_INIT_ALL;
		isFirst = false;
	}

	static_cast<MotionState*>(getMotionState())->setAttributeIndex(attributeIndex);
	reload();
}

void PhysicsObject::onUpdate(float delta)
{
}

void PhysicsObject::reload()
{
	//static float worldScale = 100.0f;
	Attribute_Physics* physicsAttribute = itrPhysics.at(static_cast<MotionState*>(getMotionState())->getAttributeIndex());
	
	setAngularVelocity(btVector3(physicsAttribute->angularVelocity.x,
								 physicsAttribute->angularVelocity.y,
								 physicsAttribute->angularVelocity.z));
	setLinearVelocity(btVector3(physicsAttribute->linearVelocity.x,
								 physicsAttribute->linearVelocity.y,
								 physicsAttribute->linearVelocity.z));
	setGravity(btVector3(physicsAttribute->gravity.x,
							physicsAttribute->gravity.y,
							physicsAttribute->gravity.z));
	setCollisionShape(CollisionShapes::Instance()->getCollisionShape(physicsAttribute->meshID));
	setMassProps(physicsAttribute->mass,
				 btVector3(0,0,0));
	activate(true);
}

void PhysicsObject::handleInput(Attribute_Input* inputAttribute)
{
	yaw_ += inputAttribute->rotation.x;
	btVector3 move = 5*btVector3(inputAttribute->position.x, 0, inputAttribute->position.y);
	move = move.rotate(btVector3(0,1,0),yaw_);
	move = btVector3(move.x(), getLinearVelocity().y(), move.z());
	setLinearVelocity(move);
	btTransform world;
	getMotionState()->getWorldTransform(world);
	world.setRotation(btQuaternion(yaw_,0,0));
	getMotionState()->setWorldTransform(world);
}