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
		case EVENT_CREATE_PROJECTILE:
			event_CreateProjectile(static_cast<Event_CreateProjectile*>(e));
			break;
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
			entityFactory.createWorldEntity(entity);
			DEBUGPRINT("ENTITYMANAGER: Created World in 7 cycles " << entity->getID());
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
};