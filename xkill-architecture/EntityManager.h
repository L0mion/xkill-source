#pragma once

#include <xkill-utilities/EntityStorage.h>
#include "EntityFactory.h"

#include <vector>
#include <iostream>

class EntityManager: public IObserver
{
private:
	EntityStorage* entities;
	EntityFactory entityFactory;

	Entity* createEntity()
	{
		return entities->createEntity();
	}

	void deleteEntity(int id)
	{
		entities->deleteEntity(id);
	}

public:
	EntityManager()
	{
		// subscribe to events
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_PROJECTILE);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_MESH);
		SUBSCRIBE_TO_EVENT(this, EVENT_REMOVE_ENTITY);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_SPAWNPOINT);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_EXPLOSIONSPHERE);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_ENTITY);

		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_WORLD);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_AMMO);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_HACK);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_LIGHT);



		entities = ATTRIBUTE_MANAGER->entities;
	}

	/**
	Handles Events for EntityManager.
	*/
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_CREATE_ENTITY:
			createSpecificEntity(static_cast<Event_CreateEntity*>(e));
			break;
		case EVENT_REMOVE_ENTITY:
			deleteEntity(static_cast<Event_RemoveEntity*>(e)->entityId);
			break;
		case EVENT_CREATE_MESH:
			event_createmesh(static_cast<Event_CreateMesh*>(e));
			break;
		case EVENT_CREATE_SPAWNPOINT:
			event_CreateSpawnPoint(static_cast<Event_CreateSpawnPoint*>(e));
			break;
		case EVENT_CREATE_EXPLOSIONSPHERE:
			event_CreateExplosionSphere(static_cast<Event_CreateExplosionSphere*>(e));
			break;
		case EVENT_CREATE_PROJECTILE:
			event_CreateProjectile(static_cast<Event_CreateProjectile*>(e));
			break;
		case EVENT_CREATE_WORLD:
			event_CreateWorld(static_cast<Event_CreateWorld*>(e));
			break;
		case EVENT_CREATE_AMMO:
			event_CreateAmmo(static_cast<Event_CreateAmmo*>(e));
			break;
		case EVENT_CREATE_HACK:
			event_CreateHack(static_cast<Event_CreateHack*>(e));
			break;
		case EVENT_CREATE_LIGHT:
			event_CreateLight(static_cast<Event_CreateLight*>(e));
			break;
		default:
			break;
		}
	}

	void update(float delta)
	{
	}

	void event_createmesh(Event_CreateMesh* e)
	{
		Entity* entity = createEntity();
		entityFactory.createMesh(entity, e);
	}

	void createSpecificEntity(Event_CreateEntity* e)
	{
		EntityType type = e->entityType;
		Entity* entity = createEntity();
		switch(type)
		{
		case PLAYER:
			entityFactory.createPlayerEntity(entity);
			DEBUGPRINT("ENTITYMANAGER: Created player entity " << entity->getID());
			break;
		case WORLD:
			entityFactory.createWorldEntity(entity, &Event_CreateWorld(Float3(0,0,0),Float4(0,0,0,1),1));
			DEBUGPRINT("ENTITYMANAGER: Created World in 7 cycles,, Satan took over and this function won't work anymore " << entity->getID());
			break;
		}
	}

	void event_CreateProjectile(Event_CreateProjectile* e)
	{
		Entity* entity = createEntity();
		entityFactory.createProjectileEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created projectile entity " << entity->getID());
	}

	void event_CreateSpawnPoint(Event_CreateSpawnPoint* e)
	{
		Entity* entity = createEntity();
		entityFactory.createSpawnPointEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created spawn point entity " << entity->getID());
	}

	void event_CreateExplosionSphere(Event_CreateExplosionSphere* e)
	{
		Entity* entity = createEntity();
		entityFactory.createExplosionSphere(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created explosion sphere entity " << entity->getID());
	}

	void event_CreateWorld(Event_CreateWorld* e)
	{
		Entity* entity = createEntity();
		entityFactory.createWorldEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created world entity " << entity->getID());
	}

	void event_CreateAmmo (Event_CreateAmmo* e)
	{
		//Entity* entity = createEntity();
		//entityFactory.createAmmoEntity(entity, e);
		//DEBUGPRINT("ENTITYMANAGER: Created ammo entity " << entity->getID());
		DEBUGPRINT("ENTITYMANAGER: Didn't create ammo entity, please implement or remove!");
	}

	void event_CreateHack (Event_CreateHack* e)
	{
		//Entity* entity = createEntity();
		//entityFactory.createHackEntity(entity, e);
		//DEBUGPRINT("ENTITYMANAGER: Created hack entity " << entity->getID());
		DEBUGPRINT("ENTITYMANAGER: Didn't create hack entity, please implement or remove!");
	}

	void event_CreateLight(Event_CreateLight* e)
	{
		Entity* entity = createEntity();
		entityFactory.createLightEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created light entity " << entity->getID());
		//EBUGPRINT("ENTITYMANAGER: Didn't create light entity, please implement or remove!");
	}
};