#pragma once
#include <xkill-utilities/IObserver.h>

class Event_EntitiesColliding;

class GameComponent :
	public IObserver
{
private:

public:
	GameComponent(void);
	~GameComponent(void);
	bool init();

	/**
	Handles Events for GameComponent.
	*/
	void onEvent(Event* e);
	void onUpdate(float delta);

	void event_EntitiesColliding(Event_EntitiesColliding* e);
};