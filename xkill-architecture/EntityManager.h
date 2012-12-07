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
	PLAYER,
	PROJECTILE
};

class EntityStorage
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
	}

	~EntityStorage()
	{
		for(unsigned i=0; i<entities.size(); i++)
		{
			entities[i].deleteAttributes();
		}
	}

	// Creates an Entity with a unique ID
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
			std::cout << "ENTITYMANAGER: Removed Entity " << id << std::endl;
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
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATEPROJECTILE);
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATEMESH);
		SUBSCRIBE_TO_EVENT(this, EVENT_REMOVE_ENTITY);
	}

	/**
	Handles Events for EntityManager.
	*/
	void onEvent(Event* e)
	{
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_CREATEPROJECTILE:
			event_CreateProjectile(static_cast<Event_createProjectileEntity*>(e));
			break;
		case EVENT_REMOVE_ENTITY:
			deleteEntity(static_cast<Event_Remove_Entity*>(e)->entityId);
			break;
		case EVENT_CREATEMESH:
			event_createmesh(static_cast<Event_createMesh*>(e));
			break;
		default:
			break;
		}
	}

	void update(float delta)
	{
	}

	void event_createmesh(Event_createMesh* e)
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
			std::cout << "ENTITYMANAGER: Created player entity " << entity->getID() << std::endl;
			break;
		}
	}

	void event_CreateProjectile(Event_createProjectileEntity* e)
	{
		Entity* entity = createEntity();
		entityFactory.createProjectileEntity(entity, e);
		std::cout << "ENTITYMANAGER: Created projectile entity " << entity->getID() << std::endl;
	}
};