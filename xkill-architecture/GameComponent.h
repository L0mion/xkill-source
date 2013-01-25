#pragma once
#include <xkill-utilities/IObserver.h>
#include <vector>

class Event_PhysicsAttributesColliding;
class Event_StartDeathmatch;
class Event_EndDeathmatch;
class Event_TransferEventsToGame;
class Entity;

struct Attribute_SpawnPoint;
struct Attribute_WeaponStats;
struct Attribute_Position;
struct Attribute_Camera;

template <class T>
class LinkFrame;

class GameComponent :
	public IObserver
{
private:
	std::vector<Entity>* allEntity;
	std::vector<Event*> levelEvents_;

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

	/**
	Handles ending of a deathmatch.
	*/
	void event_EndDeathmatch(Event_EndDeathmatch* e);

	/**
	Handles start of a deathmatch.
	*/
	void event_StartDeathmatch(Event_StartDeathmatch* e);

	/**
	Handles the transfer of the loaded leveldata
	*/
	void event_TransferEventsToGame(Event_TransferEventsToGame* e);

	/**
	Finds an unoccupied spawn point (no other player inside its radius). 
	Prioritizes spawn points that least recently spawned a player. 
	If no unoccupied spawn point is found, one is selected at random. 
	If there exists no spawn points at all, nullptr is returned.
	*/
	Attribute_SpawnPoint* findUnoccupiedSpawnPoint();

	bool switchAmmunition(Attribute_WeaponStats* weaponStats);
	bool switchFiringMode(Attribute_WeaponStats* weaponStats);

	void shootProjectile(Attribute_Position* position, Attribute_Camera* camera, Attribute_WeaponStats* weaponStats);
};