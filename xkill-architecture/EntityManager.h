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
		SUBSCRIBE_TO_EVENT(this, EVENT_CREATEMESH);
	}

	/**
	Handles Events for EntityManager.
	*/
	void onEvent(Event* e)
	{
		Entity* entity;
		EventType type = e->getType();
		switch (type) 
		{
		case EVENT_CREATEPROJECTILE:
			entity = entityFactory.createProjectileEntity(static_cast<Event_createProjectile*>(e));
			addEntity(entity);
			std::cout << "ENTITYMANAGER: Created projectile entity " << entity->getID() << std::endl;
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
		Entity* entity = entityFactory.createMesh(e);
		addEntity(entity);
	}

	void removeEntity(int index)
	{
		std::cout << "ENTITYMANAGER: Removed Entity " << entities[index]->getID() << std::endl;
		entities[index]->deleteAttributes();
		delete entities[index];
		entities.erase(entities.begin()+index);	
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