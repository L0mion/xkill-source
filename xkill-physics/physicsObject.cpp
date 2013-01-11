#include "physicsObject.h"

#include <btBulletDynamicsCommon.h>

#include <xkill-utilities/AttributeManager.h>

#include "collisionShapes.h"
#include "motionState.h"


ATTRIBUTES_DECLARE_ALL;
static bool isFirst = true;



PhysicsObject::PhysicsObject() : btRigidBody(0, new MotionState(0), nullptr)
{
	
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

	attributeIndex_ = attributeIndex;
	static_cast<MotionState*>(getMotionState())->setAttributeIndex(attributeIndex);
	reload();
}

void PhysicsObject::onUpdate(float delta)
{
	btVector3 t = getLinearVelocity();
	int a =1;
}

void PhysicsObject::reload()
{
	static float worldScale = 100.0f;
	Attribute_Physics* physicsAttribute = itrPhysics.at(attributeIndex_);
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
