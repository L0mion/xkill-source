#pragma once

#include "RenderComponent.h"
#include "SoundComponent.h"
#include "PhysicsComponent.h"
#include <vector>

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