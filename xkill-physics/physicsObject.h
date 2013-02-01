#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <BulletDynamics/Dynamics/btRigidBody.h>

class PhysicsObject
	: public btRigidBody
{
private:
	virtual btVector3 subClassCalculateLocalInertiaHook(btScalar mass);		//! May be overridden by subclasses. Is called from init.
	virtual btCollisionShape* subClassSpecificCollisionShape();				//! May be overridden by subclasses. Is called from the end of init.
	virtual bool subClassSpecificInitHook();								//! May be overridden by subclasses. Is called from the end of init.
protected:
		unsigned int attributeIndex_;										//!< Specifies which PhysicsObject is synchronized with which physics attribute.
		unsigned int collisionFilterGroup_;									//!< The filter group the object belongs to, ex: Attribute_Physics::WORLD.

		btVector3 localInertiaBasedOnCollisionShapeAndMass(btScalar mass);	//!< Called from "subClassCalculateLocalInertiaHook" by subclasses.
		btVector3 zeroLocalInertia();										//!< Called from "subClassCalculateLocalInertiaHook" by subclasses.
public:
	PhysicsObject();
	virtual ~PhysicsObject();
	//! Init PhysicsObject and call subClassSpecificInitHook()
	/*!
	\param attributeIndex The physics attribute index.
	*/
	bool init(unsigned int attributeIndex,unsigned int collisionFilterGroup);
	unsigned int getAttributeIndex() const;  //!< Returns the attribute index that the physicsobject maps to
	unsigned int getCollisionFilterGroup() const;  //!< Returns the filter group the object belongs to, ex: Attribute_Physics::WORLD
	
	virtual void onUpdate(float delta);
	virtual void handleOutOfBounds(); //!< Standard out of bounds handling: move object to a new position
};

#endif