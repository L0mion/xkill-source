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
	//CAMERA (refer to createPlayerEntity)
	PROJECTILE
};

class EntityManager
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
	}

	void update(float delta)
	{
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
		//case CAMERA:
		//	entity = entityFactory.createEntity_Camera();
		//	std::cout << "ENTITYMANAGER: Created camera entity " << entity->getID() << std::endl;
		//	break;
		case PROJECTILE:
			entity = entityFactory.createProjectileEntity();
			std::cout << "ENTITYMANAGER: Created projectile entity " << entity->getID() << std::endl;
			break;
		}
		addEntity(entity);
	}
};