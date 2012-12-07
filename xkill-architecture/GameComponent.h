#pragma once
#include <xkill-utilities/IObserver.h>

class Event_PhysicsAttributesColliding;

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

	void event_PhysicsAttributesColliding(Event_PhysicsAttributesColliding* e);
};