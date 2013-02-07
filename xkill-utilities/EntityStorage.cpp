#include "EntityStorage.h"

EntityStorage::EntityStorage()
{
	// Creates Entity 0.
	// IMPORTANT: Entity 0 is used to mark deleted
	// Attributes and shouldn't be used in the game.
	createEntity();

	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_GET_ENTITIES);
	SUBSCRIBE_TO_EVENT(this, EVENT_CREATE_ENTITY);
}

EntityStorage::~EntityStorage()
{
	for(unsigned i=0; i<entities.size(); i++)
	{
		entities[i].deleteAttributes();
		entities[i].clean();
	}
}

void EntityStorage::event_GetEntities( Event_GetEntities* e )
{
	e->entities = &entities;
}

void EntityStorage::onEvent( Event* e )
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

Entity* EntityStorage::createEntity()
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

Entity* EntityStorage::at(int index)
{
	return &entities[index];
}

void EntityStorage::deleteEntity( int id )
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
