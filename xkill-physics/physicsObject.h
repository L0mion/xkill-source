#ifndef XKILL_PHYSICS_PHYSICSOBJECT
#define XKILL_PHYSICS_PHYSICSOBJECT

#include <BulletDynamics/Dynamics/btRigidBody.h>

class btManifoldPoint;
class btCollisionObject;
struct Attribute_Input;

class PhysicsObject : public btRigidBody
{
private:
	float yaw_;
public:
	PhysicsObject(unsigned int attributeIndex);
	~PhysicsObject();
	void init(unsigned int attributeIndex);
	virtual void onUpdate(float delta);
	virtual void reload();
	virtual void handleInput(Attribute_Input* inputAttribute);
};

#endif