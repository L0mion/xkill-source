#pragma once

#include "RenderComponent.h"
#include "SoundComponent.h"
#include "PhysicsComponent.h"
#include <vector>


/// Is responsible for updating Components in a certain order
/** 
Some Components such as RenderComponent and PhysicsComponent need to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order. The InputComponent 
should run before the RenderComponent to hide latency. This should 
be the responsibility of the ComponentManager.

\ingroup achitecture
*/

class ComponentManager
{
private:
	RenderComponent render;
	PhysicsComponent physics;
	SoundComponent sound;
public:
	ComponentManager()
	{
		std::vector<PhysicsAttribute>* attributes = AttributeManager::getInstance()->physicsAttributes.getAllAttributes();
		
		//physics.init(attributes);
	}

	void update(float delta)
	{
		sound.onUpdate(delta);
		//physics.onUpdate(delta);
		render.onUpdate(delta);
	
	}
};