#pragma once
#include <xkill-utilities/IObserver.h>
#include <vector>

class Event_PhysicsAttributesColliding;
class Entity;

class GameComponent :
	public IObserver
{
private:
	std::vector<Entity>* allEntity;
	std::vector<int>* allPhysicsOwner;

public:
	GameComponent(void);
	~GameComponent(void);
	bool init();

	/**
	Handles Events for GameComponent.
	*/
	void onEvent(Event* e);
	void onUpdate(float delta);

	/**
	Handle PhysicsAttributes collision for one of two PhysicsAttributes participating in the collision.
	*/
	void event_PhysicsAttributesColliding(Event_PhysicsAttributesColliding* e);
};