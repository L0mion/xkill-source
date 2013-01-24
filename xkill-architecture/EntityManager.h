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
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_PLAYERSPAWNPOINT);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_PICKUPABLESSPAWNPOINT);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_PICKUPABLE);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_EXPLOSIONSPHERE);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_ENTITY);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_INPUTDEVICE);

		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_WORLD);
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
		case EVENT_CREATE_PLAYERSPAWNPOINT:
			event_CreatePlayerSpawnPoint(static_cast<Event_CreatePlayerSpawnPoint*>(e));
			break;
		case EVENT_CREATE_PICKUPABLESSPAWNPOINT:
			event_CreatePickupablesSpawnPoint(static_cast<Event_CreatePickupablesSpawnPoint*>(e));
			break;
		case EVENT_CREATE_PICKUPABLE:
			event_CreatePickupable(static_cast<Event_CreatePickupable*>(e));
			break;
		case EVENT_CREATE_EXPLOSIONSPHERE:
			event_CreateExplosionSphere(static_cast<Event_CreateExplosionSphere*>(e));
			break;
		case EVENT_CREATE_PROJECTILE:
			event_CreateProjectile(static_cast<Event_CreateProjectile*>(e));
			break;
		case EVENT_CREATE_INPUTDEVICE:
			event_CreateInputDevice(static_cast<Event_CreateInputDevice*>(e));
			break;
		case EVENT_CREATE_WORLD:
			event_CreateWorld(static_cast<Event_CreateWorld*>(e));
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

	void event_CreatePlayerSpawnPoint(Event_CreatePlayerSpawnPoint* e)
	{
		Entity* entity = createEntity();
		entityFactory.createPlayerSpawnPointEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created player spawn point entity " << entity->getID());
	}

	void event_CreatePickupablesSpawnPoint(Event_CreatePickupablesSpawnPoint* e)
	{
		Entity* entity = createEntity();
		entityFactory.createPickupablesSpawnPointEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created pickupables spawn point entity " << entity->getID());
	}
	void event_CreatePickupable(Event_CreatePickupable* e)
	{
		Entity* entity = createEntity();
		entityFactory.createPickupableEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created pickupable entity " << entity->getID());
	}

	void event_CreateExplosionSphere(Event_CreateExplosionSphere* e)
	{
		Entity* entity = createEntity();
		entityFactory.createExplosionSphereEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created explosion sphere entity " << entity->getID());
	}

	void event_CreateWorld(Event_CreateWorld* e)
	{
		Entity* entity = createEntity();
		entityFactory.createWorldEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created world entity " << entity->getID());
	}

	void event_CreateInputDevice(Event_CreateInputDevice* e)
	{
		Entity* entity = createEntity();
		entityFactory.createInputDevice(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created input device entity " << entity->getID());
	}

	void event_CreateLight(Event_CreateLight* e)
	{
		Entity* entity = createEntity();
		entityFactory.createLightEntity(entity, e);
		DEBUGPRINT("ENTITYMANAGER: Created light entity " << entity->getID());
		//EBUGPRINT("ENTITYMANAGER: Didn't create light entity, please implement or remove!");
	}
};