#pragma once
#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/AttributePointer.h>
#include <vector>

class Event_PhysicsAttributesColliding;
class Event_StartDeathmatch;
class Event_EndDeathmatch;
class Event_TransferEventsToGame;
class Event_PlayerDeath;
class Entity;

struct Attribute_PlayerSpawnPoint;
struct Attribute_WeaponStats;
struct Attribute_Position;
struct Attribute_Spatial;
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

	/**
	Handles most game logic for players.
	*/
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
	Handles a players death
	*/
	void event_PlayerDeath(Event_PlayerDeath* e);

	/**
	Finds an unoccupied spawn point (no other player inside its radius). 
	Prioritizes spawn points that least recently spawned a player. 
	If no unoccupied spawn point is found, one is selected at random. 
	If there exists no spawn points at all, nullptr is returned.
	Finds an unoccupied player spawn point (no other player inside its radius). 
	Prioritizes player spawn points that least recently spawned a player. 
	If no unoccupied player spawn point is found, one is selected at random. 
	If there exists no player spawn points at all, nullptr is returned.
	*/
	AttributePtr<Attribute_PlayerSpawnPoint> findUnoccupiedSpawnPoint();

	bool switchAmmunition(AttributePtr<Attribute_WeaponStats> ptr_weaponStats);
	bool switchFiringMode(AttributePtr<Attribute_WeaponStats> ptr_weaponStats);

	void shootProjectile(AttributePtr<Attribute_Spatial> ptr_spatial, AttributePtr<Attribute_WeaponStats> ptr_weaponStats);
};