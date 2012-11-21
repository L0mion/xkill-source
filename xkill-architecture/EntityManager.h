#pragma once

#include "EntityFactory.h"
#include <vector>

class EntityManager
{
private:
	vector<Entity*> entities;
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
		cout << "ENTITYMANAGER: Removed Entity " << entities[id]->getID() << endl;
		entities[id]->deleteAttributes();
		delete entities[id];
		entities.erase(entities.begin()+id);	
	}

	~EntityManager()
	{
		for each(Entity *e in entities)
		{
			e->deleteAttributes();
			delete e;
		}
	}

	void createEntity()
	{
		Entity* e = entityFactory.createEntity_TypeA();
		addEntity(e);
		cout << "ENTITYMANAGER: Created Entity " << e->getID() << endl;
	}
};