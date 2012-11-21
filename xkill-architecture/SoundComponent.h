#pragma once

#include "IComponent.h"


class SoundComponent : public IComponent
{
private:

public:
	SoundComponent()
	{
		EventManager::getInstance()->addObserverToAll(this);
	}

	void onEvent(Event* e)
	{
		EventType type = e->getType();
		
		// This is just to demonstrate Enum can be converted to a int 
		int index = type;

		// Play sound based on index
	};

	void onUpdate(float delta)
	{
		// Update FMOD
	}
};