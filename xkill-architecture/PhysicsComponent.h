#pragma once

#include <xkill-physics/BulletPhysicsComponent.h>
#include <xkill-utilities/IComponent.h>
#include "AttributeManager.h"

/// Component to process PhysicsAttribute
/** 
\ingroup components
*/

class PhysicsComponent : public IObserver
{
private:
	//BulletPhysicsComponent* bullet_;

public:
	PhysicsComponent()
	{
		//bullet_->init(AttributeManager::getInstance()->physicsAttributes.getAllAttributes());
	}

	void onEvent(Event* e)
	{
		
	}

	void onUpdate(float delta)
	{
		//bullet_.onUpdate(delta);
	}
};