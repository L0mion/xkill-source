#pragma once

#include "EntityFactory.h"
#include <vector>
#include <iostream>

/// Manages multiple Entity in a uniform way
/** 
\ingroup achitecture
*/

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
	void update(float delta)
	{
	}

	void removeEntity(int id)
	{
		std::cout << "ENTITYMANAGER: Removed Entity " << entities[id]->getID() << std::endl;
		entities[id]->deleteAttributes();
		delete entities[id];
		entities.erase(entities.begin()+id);	
	}

	~EntityManager()
	{
		for(unsigned int i = 0; i < entities.size(); i++)
		{
			entities[i]->deleteAttributes();
			delete entities[i];
		}
	}

	void createEntity()
	{
		Entity* e = entityFactory.createEntity_TypeA();
		addEntity(e);
		std::cout << "ENTITYMANAGER: Created Entity " << e->getID() << std::endl;
	}

	void createCamera()
	{
		Entity* e = entityFactory.createEntity_Camera();
		addEntity(e);
		std::cout << "ENTITYMANAGER: Created Camera " << e->getID() << std::endl;
	}
};