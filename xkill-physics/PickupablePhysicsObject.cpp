#include "PickupablePhysicsObject.h"
#include "btBulletDynamicsCommon.h"

//#include <xkill-utilities\Util.h>//check temp
#include <xkill-utilities/AttributeManager.h>
#include "physicsUtilities.h"

AttributeIterator<Attribute_Physics> itrPhysics;
AttributeIterator<Attribute_PickupablesSpawnPoint> itrPickupablesSpawnPoint;
AttributeIterator<Attribute_Pickupable> itrPickupables;

PickupablePhysicsObject::PickupablePhysicsObject()
	: PhysicsObject()
{
	itrPhysics = ATTRIBUTE_MANAGER->physics.getIterator();
	itrPickupablesSpawnPoint = ATTRIBUTE_MANAGER->pickupablesSpawnPoint.getIterator();
	itrPickupables = ATTRIBUTE_MANAGER->pickupable.getIterator();
}

PickupablePhysicsObject::~PickupablePhysicsObject()
{
}

bool PickupablePhysicsObject::subClassSpecificInitHook()
{
	removeAfterOnUpdate = false;
	return true;
}

btVector3 PickupablePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	btVector3 onlyY = (btVector3(0.0f, 1.0f, 0.0f));
	setAngularFactor(onlyY);	//Only allow rotation around y-axis
	setLinearFactor(onlyY);		//Only allow movement up and down y-axis

	return localInertiaBasedOnCollisionShapeAndMass(mass);
}

void PickupablePhysicsObject::handleOutOfBounds()
{
	Entity* entityOfPhysicsAttributeOwner = itrPhysics.ownerAt(attributeIndex_);
	std::vector<int> pickupableIds = entityOfPhysicsAttributeOwner->getAttributes(ATTRIBUTE_PICKUPABLE);
	for(int i=0;i<pickupableIds.size();i++)
	{
		AttributePtr<Attribute_Pickupable> pickupableAttribute = itrPickupables.at(pickupableIds.at(i));
		AttributePtr<Attribute_PickupablesSpawnPoint> pickupableSpawnPointAttribute = pickupableAttribute->ptr_pickupablesSpawnPoint_creator;

		//reset two variables to make the pickupables spawn point respawn a pickupable of this type
		pickupableSpawnPointAttribute->secondsSinceLastPickup = 0.0f;
		pickupableSpawnPointAttribute->currentNrOfExistingSpawnedPickupables--;
	}

	/*Remove physics attribute after onUpdate.
	This delay is needed because when a physics attribute is removed an event is sent to PhysicsComponent which then removes the physics objects corresponding to the physics attribute.
	The problem is that the code execution path is still inside the physics object that is removed
	*/
	removeAfterOnUpdate = true;
}

void PickupablePhysicsObject::onUpdate(float delta)
{
	PhysicsObject::onUpdate(delta);

	hover(delta, 1.0f);
	
	float deltaRatio = 0.25f;
	static float timer = 0;
	timer += delta;
	float angularVelocityY = getAngularVelocity().y();
	if(abs(angularVelocityY) > 2.0f) //Slow down spinning
	{
		while(timer > deltaRatio)
		{
			setAngularVelocity(btVector3(0.0f, angularVelocityY*0.9, 0.0f)); //Gradually slow down excessive spinning
			timer -= deltaRatio;
		}
	}
	else //Set standard spinning
	{
		angularVelocityY = getAngularVelocity().y();
		if(angularVelocityY > 0.0f)
		{
			setAngularVelocity(btVector3(0.0f, 2.0f, 0.0f)); //Make pickupables spin "forward" around y-axis
		}
		else
		{
			setAngularVelocity(btVector3(0.0f, -2.0f, 0.0f)); //Make pickupables spin "backwards" around y-axis
		}
	}

//Nothing else in this function below this line, except checking of the "removeAfterOnUpdate" flag
//---------------------------------------------------------------------------------------------
	//This if statement must be the last thing in this function (refer to PickupablePhysicsObject::handleOutOfBounds())
	if(removeAfterOnUpdate)
	{
		removePhysicsAttributeCorrespondingToThisPhysicsObject();
	}
}