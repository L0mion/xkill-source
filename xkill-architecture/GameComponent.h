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
	Uniform way of starting the game
	*/
	void startGame();

	/**
	Uniform way of ending the game
	*/
	void endGame();

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
	Unload current level
	*/
	void event_UnloadLevel();

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

	/**
	Creates an aiming ray by sending a ray cast event handled by PhysicsComponent.
	Retrieves the result (what the ray hit, and the hit point) from the sent event.
	Updates the ray attribute of the current player according to the result from the sent event.
	If the current player is executing the ray is interpreted as a Laser Automatic Sniper Execution Ray and render is set to true.
	Else the aiming ray is interpreted as what the player is looking at and render is set to false.
	*/
	void updateAndInterpretAimingRay(Entity* playerEntity, AttributePtr<Attribute_Camera> ptr_camera);
	void shootProjectile(AttributePtr<Attribute_Spatial> ptr_spatial, AttributePtr<Attribute_WeaponStats> ptr_weaponStats);
};