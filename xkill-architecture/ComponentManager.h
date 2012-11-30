#pragma once

#include <xkill-physics/BulletPhysicsComponent.h>
#include "RenderComponent.h"
#include "SoundComponent.h"
#include "PhysicsComponent.h"
#include "GameComponent.h"
#include <vector>
#include "AttributeManager.h"


/// Is responsible for updating Components in a certain order
/** 
Some \ref COMPONENTS such as RenderComponent and PhysicsComponent need to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order. The InputComponent 
should run before the RenderComponent to hide latency. This should 
be the responsibility of the ComponentManager.

\ingroup ARCHITECTURE
*/

class ComponentManager
{
private:
	RenderComponent render;
	BulletPhysicsComponent* physics;
	SoundComponent sound;
	GameComponent gameComponent;
public:
	ComponentManager()
	{
		physics = new BulletPhysicsComponent(AttributeManager::getInstance()->physicsAttributes.getAllAttributes(),
											 AttributeManager::getInstance()->BoundingAttributes.getAllAttributes());
		physics->init();
	}
	~ComponentManager()
	{
		delete physics;
	}
	void update(float delta)
	{
		sound.onUpdate(delta);
		render.onUpdate(delta);
		//physics->onUpdate(delta);
		gameComponent.onUpdate(delta);
	}
};