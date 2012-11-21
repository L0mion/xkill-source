#pragma once

#include "RenderComponent.h"
#include "SoundComponent.h"
#include <vector>

class ComponentManager
{
private:
	SoundComponent sound;
	RenderComponent render;

public:
	ComponentManager()
	{

	}

	void update(float delta)
	{
		sound.onUpdate(delta);
		render.onUpdate(delta);
	}
};