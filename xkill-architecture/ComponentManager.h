#pragma once

#include "RenderComponent.h"
#include "SoundComponent.h"
#include "PhysicsComponent.h"
#include <vector>


/// Is responsible for updating Components in a certain order
/** 
Some Components such as RenderComponent and PhysicsComponent needs to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order (InputComponent 
should to run before the RenderComponent to hide latency) and this should 
be the responsiblity of the ComponentManager.

\ingroup achitecture
*/

class ComponentManager
{
private:
	SoundComponent sound;
	RenderComponent render;
	PhysicsComponent physics;
public:
	ComponentManager()
	{

	}

	void update(float delta)
	{
		sound.onUpdate(delta);
		render.onUpdate(delta);
		physics.onUpdate(delta);
	}
};