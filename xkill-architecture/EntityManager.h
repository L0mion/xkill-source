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

class EntityManager: public IObserver
{
private:
	std::vector<Entity*> entities;
	EntityFactory entityFactory;

	void addEntity(Entity* e)
	{
		entities.push_back(e);
	}

public:
	EntityManager()
	{
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATEPROJECTILE);
		SUBSCRIBE_TO_EVENT(this, EVENT_REMOVE_ENTITY);
	}

	/**
	Handles Events for EntityManager.
	*/
	void onEvent(Event* e)
	{
		Entity* entity;
		EventType type = e->getType();
		int entityId;
		switch (type) 
		{
		case EVENT_CREATEPROJECTILE:
			entity = entityFactory.createProjectileEntity(static_cast<Event_createProjectile*>(e));
			addEntity(entity);
			std::cout << "ENTITYMANAGER: Created projectile entity " << entity->getID() << std::endl;
			break;
		case EVENT_REMOVE_ENTITY:
			entityId = static_cast<Event_Remove_Entity*>(e)->entityId;
			removeEntity(entityId);
			break;
		default:
			break;
		}
	}

	void update(float delta)
	{
	}

	void removeEntity(int index)
	{
		//std::cout << "ENTITYMANAGER: Removed Entity (may not work yet) " << entities[index]->getID() << std::endl;
		//entities[index]->deleteAttributes();
		//delete entities[index];
		//entities.erase(entities.begin()+index);	
	}

	~EntityManager()
	{
		for(unsigned int i = 0; i < entities.size(); i++)
		{
			entities[i]->deleteAttributes();
			delete entities[i];
		}
	}

	void createEntity(ENTITYTYPE entityType)
	{
		Entity* entity;
		switch(entityType)
		{
		case PLAYER:
			entity = entityFactory.createPlayerEntity();
			std::cout << "ENTITYMANAGER: Created player entity " << entity->getID() << std::endl;
			break;
		}
		addEntity(entity);
	}
};