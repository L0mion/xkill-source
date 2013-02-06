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
	delete getCollisionShape();
}

bool ProjectilePhysicsObject::subClassSpecificInitHook()
{
	//Anti-tunneling using btCollisionObject::setCcdSweptSphereRadius and btCollisionObject::setCcdMotionThreshold
	Attribute_Physics* physicsAttribute = itrPhysics_ProjectilePhysicsObject.at(attributeIndex_);
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getCollisionShape(physicsAttribute->meshID);
	btVector3 boundingSphereCenter;
	float boundingSphereRadius;
	collisionShape->getBoundingSphere(boundingSphereCenter, boundingSphereRadius);
	setCcdSweptSphereRadius(boundingSphereRadius*0.5); //Calculate Continuous Collision Detection (CCD) parameter based on collision shape bounding sphere radius

	//btVector3 velocity = getLinearVelocity();
	//float speed = velocity.length();
	setCcdMotionThreshold(boundingSphereRadius);

	setFriction(0.5f);
	setRollingFriction(0.5f);
	setRestitution(1.0f);

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
		Attribute_Projectile* projectileAttribute = itrProjectile_ProjectilePhysicsObject.at(projectileId.at(i));
		if(projectileAttribute->ammunitionType == XKILL_Enums::AmmunitionType::SCATTER)
		{
			return new btSphereShape(0.15f);
		}
		else
		{
			return PhysicsObject::subClassSpecificCollisionShape();
		}
	}
}