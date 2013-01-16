#pragma once

#include "Entity.h"
#include "IObserver.h"


/// Manages multiple Entities in a uniform way.
/** 
\ingroup ARCHITECTURE
*/

class DLL_U EntityStorage : public IObserver
{
private:
	std::vector<Entity> entities;
	std::queue<int> deleted;
	int index;

public:
	EntityStorage();

	~EntityStorage();

	/**
	Gives access to Entities through a Event_GetEntities.
	*/
	void event_GetEntities(Event_GetEntities* e);

	/**
	Handles Events for EntityStorage.
	*/
	void onEvent(Event* e);

	/**
	Creates an Entity with a unique ID.
	*/
	Entity* createEntity();

	/**
	Returns the Entity at index.
	*/
	Entity* at(int index);

	/**
	Deletes an Entity based on its unique ID.
	*/
	void deleteEntity(int id);
};