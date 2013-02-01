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
	Attribute_ExplosionSphere* explosionSphereAttribute;
	std::vector<int> explosionSphereEntityId = itrPhysics.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);
	for(unsigned int i = 0; i < explosionSphereEntityId.size(); i++)
	{
		explosionSphereAttribute = itrExplosionSphere.at(explosionSphereEntityId.at(i)); 

		ammunition = mutatorSettings.getStandardAmmunition(explosionSphereAttribute->ammunitionType);
		FiringMode firingMode = mutatorSettings.getStandardFiringMode(explosionSphereAttribute->firingModeType);
		
		float initialRadius = ammunition.explosionSphereInitialRadius * firingMode.explosionSphereModifier;
		float finalRadius = ammunition.explosionSphereFinalRadius * firingMode.explosionSphereModifier;
		if(initialRadius > explosionSphereAttribute->currentRadius)
		{
			explosionSphereAttribute->currentRadius = initialRadius;
		}

		if(finalRadius > explosionSphereFinalRadius_)
		{
			explosionSphereFinalRadius_ = finalRadius;
		}

		explosionSphereExpansionRate_ = (finalRadius-initialRadius)/ammunition.explosionSphereExplosionDuration;
	}

	//Create local collision shape
	localCollisionShape_ = new btSphereShape(explosionSphereAttribute->currentRadius);
	setCollisionShape(localCollisionShape_);

	return true;
}

void ExplosionSpherePhysicsObject::onUpdate(float delta)
{
	//Expand explosion sphere according to mutator settings retrieved in "subClassSpecificInitHook()"
	std::vector<int> explosionSphereEntityId = itrPhysics.ownerAt(attributeIndex_)->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);
	for(unsigned int i = 0; i < explosionSphereEntityId.size(); i++)
	{
		Attribute_ExplosionSphere* explosionSphereAttribute = itrExplosionSphere.at(explosionSphereEntityId.at(i));
		explosionSphereAttribute->currentRadius += explosionSphereExpansionRate_*delta;
		if(explosionSphereAttribute->currentRadius >= explosionSphereFinalRadius_)
		{
			explosionSphereAttribute->currentRadius = explosionSphereFinalRadius_;
		}
		localCollisionShape_->setLocalScaling(btVector3(explosionSphereAttribute->currentRadius, explosionSphereAttribute->currentRadius, explosionSphereAttribute->currentRadius));
	}
}