#pragma once

#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include <xkill-sound/SoundComponent.h>
#include "GameComponent.h"

/// Is responsible for updating Components in a certain order
/** 
Some \ref COMPONENTS such as RenderComponent and PhysicsComponent need to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order. The InputComponent 
should run before the RenderComponent to hide latency. This should 
be the responsibility of the ComponentManager.

\ingroup ARCHITECTURE
*/

//#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

class ComponentManager
{
private:
	RenderComponent renderComponent;
	//PhysicsComponent physicsComponent;
	SoundComponent soundComponent;
	GameComponent gameComponent;

public:
	ComponentManager()
	{
		//renderComponent = NULL;
		//physicsComponent = NULL;
		//soundComponent = NULL;
		//gameComponent = NULL;
	}
	~ComponentManager()
	{
		//SAFE_DELETE(renderComponent);
		//SAFE_DELETE(physicsComponent);
		//SAFE_DELETE(soundComponent);
		//SAFE_DELETE(gameComponent);
	}

	bool init()
	{
		//gameComponent = new GameComponent();
		if(!gameComponent.init())
			return false;

		//soundComponent = new SoundComponent();
		if(!soundComponent.init())
			return false;

		//physicsComponent = new PhysicsComponent();
		//if(!physics.init(attributes))
		//	return false;
		//std::vector<PhysicsAttribute>* attributes = AttributeManager::getInstance()->physicsAttributes.getAllAttributes();

		return true;
	}
	void update(float delta)
	{
		gameComponent.onUpdate(delta);
		//soundComponent.onUpdate(delta);
		//physics.onUpdate(delta);
		//renderComponent.onUpdate(delta);
	}
};