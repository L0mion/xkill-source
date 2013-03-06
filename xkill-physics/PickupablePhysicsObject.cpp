#include "PickupablePhysicsObject.h"
#include "btBulletDynamicsCommon.h"

#include <xkill-utilities\Util.h>//check temp
#include "physicsUtilities.h"
PickupablePhysicsObject::PickupablePhysicsObject()
	: PhysicsObject()
{
}

PickupablePhysicsObject::~PickupablePhysicsObject()
{
}

bool PickupablePhysicsObject::subClassSpecificInitHook()
{
	return true;
}

btVector3 PickupablePhysicsObject::subClassCalculateLocalInertiaHook(btScalar mass)
{
	return localInertiaBasedOnCollisionShapeAndMass(mass);
}

void PickupablePhysicsObject::onUpdate(float delta)
{
	hover(delta, 1.0f);
	setLinearVelocity(btVector3(0.0f, getLinearVelocity().y(), 0.0f)); //Prevent pickupable from floating away
	setAngularVelocity(btVector3(0.0f, 1.0f, 0.0f)); //Make pickupables spin around y-axis
	//setAngularVelocity(getAngularVelocity().absolute()*0.9f); //Gradually slow down spinning

	/*
	btQuaternion q = getWorldTransform().getRotation();
	Float4 rotation = Float4(q.x(), q.y(), q.z(), q.w());

	//Float3 pos = itrPhysics3.at(attributeIndex_)
	Float3 position = convert(&getWorldTransform().getOrigin());
	Float3 up = Float3(0.0f, 1.0f, 0.0f);
	Float3 look = Float3(0.0f, 0.0f, 1.0f);
	Float3 upRotated = up.rotateWithQuaternion(rotation);
	Float3 lookRotated = look.rotateWithQuaternion(rotation);
	Float4 upRotation = upRotation.quaternionFromVector(position, upRotated, up);


	if(upRotated == up)
	{
		setAngularVelocity(btVector3(0.0f, 1.0f, 0.0f)); //Make pickupables spin around y-axis
	}
	else
	{
		rotation = rotation.slerp(upRotation, 1.0f);

		q.setX(rotation.x);
		q.setY(rotation.y);
		q.setZ(rotation.z);
		q.setW(rotation.w);

		q.normalize();

		//q.setX(0.0f);
		//q.setY(0.0f);
		//q.setZ(0.0f);
		//q.setW(1.0f);
		//q.sete

		btTransform t = getWorldTransform();
		t.setRotation(q);
		setWorldTransform(t);
	}

	//setAngularVelocity(btVector3(0.0f, 1.0f, 0.0f)); //Make pickupables spin around y-axis
	*/
}