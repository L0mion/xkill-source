#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <BulletDynamics/Dynamics/btRigidBody.h>

class PhysicsObject : public btRigidBody
{
private:
	unsigned int attributeIndex_;
public:
	PhysicsObject();
	~PhysicsObject();
	void init(unsigned int attributeIndex);
	void preStep();
	void postStep();
};

#endif