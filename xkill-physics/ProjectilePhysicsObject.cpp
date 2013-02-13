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

	//Calculate Continuous Collision Detection (CCD) parameters based on collision shape bounding sphere radius
	setCcdSweptSphereRadius(boundingSphereRadius*0.5f); 
	setCcdMotionThreshold(boundingSphereRadius);

	//Involve speed in CCD?
	//btVector3 velocity = getLinearVelocity();
	//float speed = velocity.length();

	//Bullet Physics settings based on ammunitionType
	Entity* entity = itrPhysics_ProjectilePhysicsObject.ownerAt(attributeIndex_);
	std::vector<int> projectileId = entity->getAttributes(ATTRIBUTE_PROJECTILE);
	for(unsigned i=0;i<projectileId.size();i++)
	{
		AttributePtr<Attribute_Projectile> ptr_projectile = itrProjectile_ProjectilePhysicsObject.at(projectileId.at(i));

		switch(ptr_projectile->ammunitionType)
		{
		case XKILL_Enums::AmmunitionType::BULLET:
			setFriction(btScalar(1.0f));
			setRollingFriction(btScalar(0.1f));
			setRestitution(btScalar(10.0f));
			break;
		case XKILL_Enums::AmmunitionType::EXPLOSIVE:
			break;
		case XKILL_Enums::AmmunitionType::SCATTER:
			setFriction(btScalar(0.5f));
			setRollingFriction(btScalar(0.5f));
			setRestitution(btScalar(0.1f));
			break;
		}
	}

	return true;
}

btVector3 ProjectilePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}

btCollisionShape* ProjectilePhysicsObject::subClassSpecificCollisionShape()
{
	Entity* entity = itrPhysics_ProjectilePhysicsObject.ownerAt(attributeIndex_);
	std::vector<int> projectileId = entity->getAttributes(ATTRIBUTE_PROJECTILE);
	for(unsigned i=0;i<projectileId.size();i++)
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