#include "AttributeManager.h"
#include "EntityStorage.h"



void AttributeManager::createEntityStorage()
{
	entities = new EntityStorage;
}

AttributeManager::~AttributeManager()
{
	delete entities;
	delete settings;
}

AttributeManager* AttributeManager::instance()
{
	static AttributeManager instance;
	static bool test = true;
	if(test)
	{
		test = false;
	}
	return &instance;
}