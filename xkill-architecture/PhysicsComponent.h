#pragma once

#include <xkill-physics/physicsComponent.h>
#include <xkill-utilities/IComponent.h>
#include "AttributeManager.h"

class PhysicsComponent : public IComponent
{
private:
	BulletPhysicsComponent bullet_;

public:
	PhysicsComponent()
	{
		bullet_.Init(AttributeManager::getInstance()->physicsAttributes.getAllAttributes());
	}

	void onEvent(Event* e)
	{
		
	}

	void onUpdate(float delta)
	{
		bullet_.onUpdate(delta);
	}
};