#include "HoverPhysicsObject.h"


#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <xkill-utilities/AttributeManager.h>

#include "physicsUtilities.h"

ATTRIBUTES_DECLARE_ALL

HoverPhysicsObject::HoverPhysicsObject()
	: PhysicsObject()
{
	localCollisionShape_ = nullptr;
	ATTRIBUTES_INIT_ALL
}

HoverPhysicsObject::~HoverPhysicsObject()
{
	if(localCollisionShape_ != nullptr)
	{
		delete localCollisionShape_;
	}
}

bool HoverPhysicsObject::subClassSpecificInitHook()
{
	delete getMotionState();
	setMotionState(nullptr);
	//Get information from Attribute_ExplosionSphere and MutatorSettings and store in class variables accessible from "onUpdate()"
	//Create local collision shape
	localCollisionShape_ = new btBoxShape(btVector3(0.2f,0.75f,0.2f));
	setCollisionShape(localCollisionShape_);
	setCollisionFlags(CF_NO_CONTACT_RESPONSE);
	return true;
	btQuaternion q;
	btVector3 pos;
	q = btQuaternion(0,0,0);
	pos = convert(itrPlayer.at(attributeIndex_)->ptr_render->ptr_spatial->ptr_position->position);
	btTransform world;
	world.setRotation(q);
	world.setOrigin(pos);
	setWorldTransform(world);
}

void HoverPhysicsObject::onUpdate(float delta,btDynamicsWorld* dynamicWorld)
{
	btQuaternion q;
	btVector3 pos;
	q = btQuaternion(0,0,0);
	pos = convert(itrPlayer.at(attributeIndex_)->ptr_render->ptr_spatial->ptr_position->position);
	btTransform world;
	world.setRotation(q);
	world.setOrigin(pos);
	setWorldTransform(world);
}