#include "ProjectilePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <xkill-utilities/AttributeManager.h>
#include "collisionShapes.h"

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

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

	//Testing in progress

	//setCcdSweptSphereRadius(SETTINGS->soundVolume); 
	//setCcdMotionThreshold(SETTINGS->occlusionRadius);

	//setCcdSweptSphereRadius(0.0f); 
	//setCcdMotionThreshold(0.0f);

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
	btCollisionShape* collisionShape = nullptr;

	Entity* entity = itrPhysics_ProjectilePhysicsObject.ownerAt(attributeIndex_);
	std::vector<int> projectileId = entity->getAttributes(ATTRIBUTE_PROJECTILE);
	for(unsigned i=0;i<projectileId.size();i++)
	{
		AttributePtr<Attribute_Projectile> ptr_projectile = itrProjectile_ProjectilePhysicsObject.at(projectileId.at(i));
		switch(ptr_projectile->ammunitionType)
		{
		case XKILL_Enums::AmmunitionType::SCATTER:
			//return PhysicsObject::subClassSpecificCollisionShape();
			collisionShape = CollisionShapes::Instance()->scatterProjectileCollisionShape;
			break;
		case XKILL_Enums::AmmunitionType::BULLET:
			return PhysicsObject::subClassSpecificCollisionShape();
			break;
		case XKILL_Enums::AmmunitionType::EXPLOSIVE:
			return PhysicsObject::subClassSpecificCollisionShape();
			break;
		}
	}
	return collisionShape;
}

void ProjectilePhysicsObject::handleOutOfBounds()
{
	removePhysicsAttributeCorrespondingToThisPhysicsObject();
}

void ProjectilePhysicsObject::onUpdate()
{
}