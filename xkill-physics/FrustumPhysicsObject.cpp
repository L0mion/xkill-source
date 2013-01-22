#include "FrustumPhysicsObject.h"

//#include <BulletCollision/CollisionShapes/btCollisionShape.h>
//#include <BulletCollision/CollisionShapes/btSphereShape.h>

//#include "collisionShapes.h"
#include <xkill-utilities/EventManager.h>
#include "CollisionShapes.h"
#include "physicsUtilities.h"
AttributeIterator<Attribute_Camera> itrCamera_3;
AttributeIterator<Attribute_Spatial> itrSpatial_3;
AttributeIterator<Attribute_Position> itrPosition_3;


FrustumPhysicsObject::FrustumPhysicsObject()
	: PhysicsObject()
{
	itrCamera_3 = ATTRIBUTE_MANAGER->camera.getIterator();
	itrSpatial_3 = ATTRIBUTE_MANAGER->spatial.getIterator();
	itrPosition_3 = ATTRIBUTE_MANAGER->position.getIterator();
}

FrustumPhysicsObject::~FrustumPhysicsObject()
{
}

void FrustumPhysicsObject::onUpdate(float delta)
{
	setCollisionFlags(getCollisionFlags() | CF_NO_CONTACT_RESPONSE);
	//Move code to update camera here
	btMatrix3x3 view = convert(itrCamera_3.at(attributeIndex_)->mat_view);
	btVector3 pos = convert(itrPosition_3.at(itrSpatial_3.at(itrCamera_3.at(attributeIndex_)->ptr_spatial)->ptr_position)->position);
	btQuaternion q;
	view.getRotation(q);
	btTransform world = getWorldTransform();
	world.setRotation(q);
	world.setOrigin(pos);
	setWorldTransform(world);
}

bool FrustumPhysicsObject::frustumInit(unsigned int attributeIndex,unsigned int collisionFilterGroup)
{
	if(attributeIndex < 0)
	{
		return false;
	}
	attributeIndex_ = attributeIndex;
	collisionFilterGroup_ = collisionFilterGroup;

	//Get the init data from a physics attribute
	Attribute_Camera* cameraAttribute = itrCamera_3.at(attributeIndex);

	//Resolve mass, local inertia of the collision shape, and also the collision shape itself.
	btVector3 localInertia;
	localInertia.setZero();
	btCollisionShape* collisionShape = CollisionShapes::Instance()->getFrustrumShape(attributeIndex_);
	btScalar mass = static_cast<btScalar>(0);
	//if(physicsAttribute->mass != 0.0f) //calling "setMassProps()" below will set the CF_STATIC_OBJECT flag to true for the btRigidBody if the mass is zero
	//{
	//	collisionShape->calculateLocalInertia(mass, localInertia);
	//}
	setMassProps(mass, localInertia); //Set inverse mass and inverse local inertia
	setCollisionShape(collisionShape);

	/*if((getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT))
	{*/
		btTransform world;

		Attribute_Spatial* spatialAttribute = itrSpatial_3.at(itrCamera_3.at(attributeIndex_)->ptr_spatial);
 		Attribute_Position* positionAttribute = itrPosition_3.at(spatialAttribute->ptr_position);
 		world.setOrigin(convert(positionAttribute->position));
		world.setRotation(convert(spatialAttribute->rotation));
		setWorldTransform(world);  //Static physics objects: transform once
	//}
	//else
	//{
	//	//Non-static physics objects: let a derived btMotionState handle transforms.
	//	MotionState* customMotionState = new MotionState(attributeIndex);
	//	setMotionState(customMotionState);

	//	setAngularVelocity(btVector3(physicsAttribute->angularVelocity.x,
	//									physicsAttribute->angularVelocity.y,
	//									physicsAttribute->angularVelocity.z));
	//	setLinearVelocity(btVector3(physicsAttribute->linearVelocity.x,
	//									physicsAttribute->linearVelocity.y,
	//									physicsAttribute->linearVelocity.z));
	//	//Gravity is set after "addRigidBody" for non-static physics objects
	//}

	/*if(physicsAttribute->collisionResponse)
	{*/
	//	setCollisionFlags(getCollisionFlags() & ~CF_NO_CONTACT_RESPONSE); //Activate collision response
	//}
	//else
	//{
		setCollisionFlags(getCollisionFlags() | CF_NO_CONTACT_RESPONSE); //Deactivate collision response
	//}
	//
	//return subClassSpecificInitHook();
	return true;
}