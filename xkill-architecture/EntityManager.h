#pragma once

#include "EntityFactory.h"

#include <vector>
#include <iostream>

/// Manages multiple Entities in a uniform way.
/** 
\ingroup ARCHITECTURE
*/

enum ENTITYTYPE
{
	WORLD,
	PLAYER,
	PROJECTILE
};

class EntityStorage : public IObserver
{
private:
	std::vector<Entity> entities;
	std::queue<int> deleted;
	int index;

public:
	EntityStorage()
	{
		// Creates Entity 0.
		// IMPORTANT: Entity 0 is used to mark deleted
		// Attributes and shouldn't be used in the game.
		createEntity();

		// subscribe to events
		SUBSCRIBE_TO_EVENT(this, EVENT_GET_ENTITIES);
	}

	~EntityStorage()
	{
		for(unsigned i=0; i<entities.size(); i++)
		{
			entities[i].deleteAttributes();
		}
	}

	/**
	Gives access to Entities through a Event_GetEntities.
	*/
	void event_GetEntities(Event_GetEntities* e)
	{
		e->entities = &entities;
	}

	/**
	Handles Events for EntityStorage.
	*/
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_GET_ENTITIES:
			event_GetEntities(static_cast<Event_GetEntities*>(e));
			break;
		default:
			break;
		}
	}

	/**
	Creates an Entity with a unique ID.
	*/
	Entity* createEntity()
	{
		// TRUE: Reuse Entity
		if(deleted.size() > 0)
		{
			index = deleted.front();
			deleted.pop();
		}
		// ELSE: Create new Entity
		else
		{
			index = (int)entities.size();
			entities.push_back(Entity(index));
		}

		return &entities[index];
	}

	/**
	Deletes an Entity based on its unique ID.
	*/
	void deleteEntity(int id)
	{
		// TRUE: Make sure no one is trying to delete "Entity 0"
		if(id == 0)
		{
			std::string message = "Trying to delete 'Entity 0'. Entity 0 is used to mark deleted Attributes and is not allowed to be deleted.";
			SHOW_MESSAGEBOX(message);
		}
		// ELSE: Delete Entity
		else
		{
			entities[id].deleteAttributes();
			deleted.push(id);
			DEBUGPRINT("ENTITYMANAGER: Removed Entity " << id);
		}
	}
};

class EntityManager: public IObserver
{
private:
	EntityStorage entities;
	EntityFactory entityFactory;

	Entity* createEntity()
	{
		return entities.createEntity();
	}

	void deleteEntity(int id)
	{
		entities.deleteEntity(id);
	}

public:
	EntityManager()
	{
		// subscribe to events
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_PROJECTILE);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_MESH);
		SUBSCRIBE_TO_EVENT(this, EVENT_REMOVE_ENTITY);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_SPAWNPOINT);
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
		case EVENT_REMOVE_ENTITY:
			deleteEntity(static_cast<Event_RemoveEntity*>(e)->entityId);
			break;
		case EVENT_CREATE_MESH:
			event_createmesh(static_cast<Event_CreateMesh*>(e));
			break;
		case EVENT_CREATE_SPAWNPOINT:
			event_CreateSpawnPoint(static_cast<Event_CreateSpawnPoint*>(e));
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

	void createSpecificEntity(ENTITYTYPE entityType)
	{
		Entity* entity = createEntity();
		switch(entityType)
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
};