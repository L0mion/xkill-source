#pragma once
#include <xkill-utilities/IObserver.h>
#include <vector>

class Event_PhysicsAttributesColliding;
class Event_EndDeathmatch;
class Entity;

struct PlayerAttribute;
struct HealthAttribute;
struct CameraAttribute;
struct InputAttribute;
struct RenderAttribute;
struct SpatialAttribute;
struct PositionAttribute;
struct ProjectileAttribute;
struct PhysicsAttribute;
struct SpawnPointAttribute;
struct WeaponStatsAttribute;
struct DamageAttribute;
struct ExplosionSphereAttribute;

class GameComponent :
	public IObserver
{
private:
	std::vector<Entity>* allEntity;
	std::vector<int>* allPhysicsOwner;

	std::vector<PlayerAttribute>* playerAttributes_;		
	std::vector<HealthAttribute>* healthAttributes_;		
	std::vector<CameraAttribute>* cameraAttributes_;		
	std::vector<InputAttribute>* inputAttributes_;			
	std::vector<RenderAttribute>* renderAttributes_;		
	std::vector<SpatialAttribute>* spatialAttributes_;		
	std::vector<PositionAttribute>* positionAttributes_;	
	std::vector<ProjectileAttribute>* projectileAttributes_;
	std::vector<PhysicsAttribute>* physicsAttributes_;
	std::vector<SpawnPointAttribute>* spawnPointAttributes_;
	std::vector<WeaponStatsAttribute>* weaponStatsAttributes_;
	std::vector<DamageAttribute>* damageAttributes_;
	std::vector<ExplosionSphereAttribute>* explosionSphereAttributes_;

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
	Finds an unoccupied spawn point (no other player inside its radius). Prioritizes spawn points that least recently spawned a player. If no unoccupied spawn point is found, one is selected at random. If there exists no spawn points at all, nullptr is returned.
	*/
	SpawnPointAttribute* findUnoccupiedSpawnPoint();
};