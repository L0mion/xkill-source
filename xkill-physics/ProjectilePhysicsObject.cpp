#include "ProjectilePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <xkill-utilities/AttributeManager.h>
#include "collisionShapes.h"

AttributeIterator<Attribute_Physics> itrPhysics_ProjectilePhysicsObject;
AttributeIterator<Attribute_Projectile> itrProjectile_ProjectilePhysicsObject;

ProjectilePhysicsObject::ProjectilePhysicsObject()
	: PhysicsObject()
{
	itrPhysics_ProjectilePhysicsObject = ATTRIBUTE_MANAGER->physics.getIterator();
	itrProjectile_ProjectilePhysicsObject = ATTRIBUTE_MANAGER->projectile.getIterator();
}

ProjectilePhysicsObject::~ProjectilePhysicsObject()
{
	if(localCollisionShape_)
	{
		delete getCollisionShape();
	}
}

bool ProjectilePhysicsObject::subClassSpecificInitHook()
{
	//Anti-tunneling using btCollisionObject::setCcdSweptSphereRadius and btCollisionObject::setCcdMotionThreshold
	AttributePtr<Attribute_Physics> ptr_physics = itrPhysics_ProjectilePhysicsObject.at(attributeIndex_);
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getCollisionShape(ptr_physics->meshID);
	btVector3 boundingSphereCenter;
	float boundingSphereRadius;
	collisionShape->getBoundingSphere(boundingSphereCenter, boundingSphereRadius);
	setCcdSweptSphereRadius(boundingSphereRadius*0.5); //Calculate Continuous Collision Detection (CCD) parameter based on collision shape bounding sphere radius

	//btVector3 velocity = getLinearVelocity();
	//float speed = velocity.length();
	setCcdMotionThreshold(boundingSphereRadius);

	setFriction(btScalar(0.5f));
	setRollingFriction(btScalar(0.5f));
	setRestitution(btScalar(1.0f));

	return true;
}

btVector3 ProjectilePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}

btCollisionShape* ProjectilePhysicsObject::subClassSpecificCollisionShape()
{
	btCollisionShape* collisionShape;
	Entity* entity = itrPhysics_ProjectilePhysicsObject.ownerAt(attributeIndex_);
	std::vector<int> projectileId = entity->getAttributes(ATTRIBUTE_PROJECTILE);
	for(int i=0;i<projectileId.size();i++)
	{
		AttributePtr<Attribute_Projectile> ptr_projectile = itrProjectile_ProjectilePhysicsObject.at(projectileId.at(i));
		if(ptr_projectile->ammunitionType == XKILL_Enums::AmmunitionType::SCATTER)
		{
			localCollisionShape_ = true;
			return new btSphereShape(0.15f);
		}
		else
		{
			localCollisionShape_ = false;
			return PhysicsObject::subClassSpecificCollisionShape();
		}
	}
}