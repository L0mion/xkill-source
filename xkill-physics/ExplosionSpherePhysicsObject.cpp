#include "ExplosionSpherePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <xkill-utilities/AttributeManager.h>

#include "collisionShapes.h"

ATTRIBUTES_DECLARE_ALL

ExplosionSpherePhysicsObject::ExplosionSpherePhysicsObject()
	: PhysicsObject()
{
	ATTRIBUTES_INIT_ALL
}

ExplosionSpherePhysicsObject::~ExplosionSpherePhysicsObject()
{
}

bool ExplosionSpherePhysicsObject::subClassSpecificInitHook()
{
	//setCollisionShape(CollisionShapes::Instance()->getCollisionShape(143250));
	
	std::vector<int> indices = itrPhysics.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);

	float radius = 0.0f;

	for(unsigned int i = 0; i < indices.size(); i++)
	{
		Attribute_ExplosionSphere* explosionSphere = itrExplosionSphere.at(i);

		if(explosionSphere->radius > radius)
			radius = explosionSphere->radius;
	}

	btCollisionShape* collisionShape = getCollisionShape();
	collisionShape->setLocalScaling(btVector3(radius, radius, radius));
	
	btSphereShape* sphere = static_cast<btSphereShape*>(collisionShape);

	Attribute_Physics* physicsAttribute = itrPhysics.at(attributeIndex_);
	physicsAttribute->reloadDataIntoBulletPhysics = true;

	return true;
}