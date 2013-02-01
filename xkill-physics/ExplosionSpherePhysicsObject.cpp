#include "ExplosionSpherePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <xkill-utilities/AttributeManager.h>

#include "collisionShapes.h"

ATTRIBUTES_DECLARE_ALL

ExplosionSpherePhysicsObject::ExplosionSpherePhysicsObject()
	: PhysicsObject()
{
	localCopyOfCollisionShape_ = nullptr;
	radius = 0.0f;
	ATTRIBUTES_INIT_ALL
}

ExplosionSpherePhysicsObject::~ExplosionSpherePhysicsObject()
{
	if(localCopyOfCollisionShape_ != nullptr)
	{
		delete localCopyOfCollisionShape_;
	}
}

bool ExplosionSpherePhysicsObject::subClassSpecificInitHook()
{
	//setCollisionShape(CollisionShapes::Instance()->getCollisionShape(143250));
	
	std::vector<int> indices = itrPhysics.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);

	for(unsigned int i = 0; i < indices.size(); i++)
	{
		Attribute_ExplosionSphere* explosionSphere = itrExplosionSphere.at(i);

		if(explosionSphere->radius > radius)
			radius = explosionSphere->radius;
	}

	//Retrieve the collision shape
	btCollisionShape* collisionShape = getCollisionShape();
	btSphereShape* sphere = static_cast<btSphereShape*>(collisionShape);
	
	//Create local copy of the collision shape
	localCopyOfCollisionShape_ = new btSphereShape(*sphere);
	setCollisionShape(localCopyOfCollisionShape_);

	//Scale the local copy of the collosion shape
	localCopyOfCollisionShape_->setLocalScaling(btVector3(radius, radius, radius));
	
	return true;
}

void ExplosionSpherePhysicsObject::onUpdate(float delta)
{
	radius += delta;
	localCopyOfCollisionShape_->setLocalScaling(btVector3(radius, radius, radius));
}