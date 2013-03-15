#include "ExplosionSpherePhysicsObject.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/MutatorSettings.h>

#include "collisionShapes.h"

ATTRIBUTES_DECLARE_ALL

ExplosionSpherePhysicsObject::ExplosionSpherePhysicsObject()
	: PhysicsObject()
{
	localCollisionShape_ = nullptr;
	explosionSphereExpansionRate_ = 0.0f;
	ATTRIBUTES_INIT_ALL
}

ExplosionSpherePhysicsObject::~ExplosionSpherePhysicsObject()
{
	if(localCollisionShape_ != nullptr)
	{
		delete localCollisionShape_;
	}
}

bool ExplosionSpherePhysicsObject::subClassSpecificInitHook()
{
	//Get information from Attribute_ExplosionSphere and MutatorSettings and store in class variables accessible from "onUpdate()"
	MutatorSettings mutatorSettings;
	Ammunition ammunition;
	AttributePtr<Attribute_ExplosionSphere> ptr_explosionSphere;
	std::vector<int> explosionSphereEntityId = itrPhysics.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);
	for(unsigned int i = 0; i < explosionSphereEntityId.size(); i++)
	{
		ptr_explosionSphere = itrExplosionSphere.at(explosionSphereEntityId.at(i)); 

		ammunition = mutatorSettings.getStandardAmmunition(ptr_explosionSphere->ammunitionType);
		FiringMode firingMode = mutatorSettings.getStandardFiringMode(ptr_explosionSphere->firingModeType);
		
		float initialRadius = ammunition.explosionSphereInitialRadius * firingMode.explosionSphereModifier;
		float finalRadius = ammunition.explosionSphereFinalRadius * firingMode.explosionSphereModifier;
		if(initialRadius > ptr_explosionSphere->currentRadius)
		{
			ptr_explosionSphere->currentRadius = initialRadius;
		}

		if(finalRadius > explosionSphereFinalRadius_)
		{
			explosionSphereFinalRadius_ = finalRadius;
		}

		explosionSphereExpansionRate_ = (finalRadius-initialRadius)/ammunition.explosionSphereExplosionDuration;
	}

	//Create local collision shape
	localCollisionShape_ = new btSphereShape(1.0f);
	setCollisionShape(localCollisionShape_);

	return true;
}

btVector3 ExplosionSpherePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return zeroLocalInertia();
}

void ExplosionSpherePhysicsObject::onUpdate(float delta)
{
	//Expand explosion sphere according to mutator settings retrieved in "subClassSpecificInitHook()"
	std::vector<int> explosionSphereEntityId = itrPhysics.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);
	for(unsigned int i = 0; i < explosionSphereEntityId.size(); i++)
	{
		AttributePtr<Attribute_ExplosionSphere> ptr_explosionSphere = itrExplosionSphere.at(explosionSphereEntityId.at(i));
		ptr_explosionSphere->currentRadius += explosionSphereExpansionRate_*delta;
		if(ptr_explosionSphere->currentRadius >= explosionSphereFinalRadius_)
		{
			ptr_explosionSphere->currentRadius = explosionSphereFinalRadius_;
		}
		itrPhysics.at(attributeIndex_)->ptr_spatial->scale = Float3(ptr_explosionSphere->currentRadius,ptr_explosionSphere->currentRadius,ptr_explosionSphere->currentRadius);
		localCollisionShape_->setLocalScaling(btVector3(ptr_explosionSphere->currentRadius, ptr_explosionSphere->currentRadius, ptr_explosionSphere->currentRadius));
	}
}