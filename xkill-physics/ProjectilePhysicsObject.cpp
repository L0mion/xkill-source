#include "ProjectilePhysicsObject.h"

//#include <xkill-utilities/AttributeManager.h>
//AttributeIterator<Attribute_Physics> itrPhysics_ProjectilePhysicsObject;

ProjectilePhysicsObject::ProjectilePhysicsObject()
	: PhysicsObject()
{
	//itrPhysics_ProjectilePhysicsObject = ATTRIBUTE_MANAGER->physics.getIterator();
}

ProjectilePhysicsObject::~ProjectilePhysicsObject()
{
}

bool ProjectilePhysicsObject::subClassSpecificInitHook()
{
	//Attribute_Physics* physicsAttribute = itrPhysics_ProjectilePhysicsObject.at(attributeIndex_);

	
	btVector3 velocity = getLinearVelocity();
	float speed = velocity.length();

	//Box shape: 0.1, 0.1, 0.494
	//Projectile speed: 5
	//Wall thickness: 0.01
	//dynamicsWorld_->stepSimulation(delta,10);
	//Working settings:
	setCcdMotionThreshold(1.0f);
	setCcdSweptSphereRadius(0.2f);

	//

	//setCcdMotionThreshold(0.001f);
	//setCcdSweptSphereRadius(0.001f);
	return true;
}