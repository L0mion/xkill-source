#include "AttributeManager.h"
#include "EntityStorage.h"
#include "DataItem.h"


void AttributeManager::createEntityStorage()
{
	entities = new EntityStorage;
}

AttributeManager::~AttributeManager()
{
	delete entities;
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

float Settings::timeScale()
{
	return _timeScale;
}

void Settings::setTimeScale( float timeScale )
{
	_timeScale = timeScale;

	if(_timeScale > 2.0f)
		_timeScale = 2.0f;

	if(_timeScale < 0.01f)
		_timeScale = 0.01f;
}

Settings::Settings()
{
	timeUntilScheduling = 0.0f;
	numErrors = 0;
	soundVolume = 0.1f;
	numPlayers = 1;
	_timeScale = 1.0f;
	trueDeltaTime = 0.0f;
	currentLevel = "TestArena";
	timeLimit = 0.0f;
	cycleLimit = 35;
	showDebugPhysics = false;
}

DataItemList* Settings::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(soundVolume, "soundVolume");

	return list;
}

void Settings::saveTo( DataItemList* list )
{
	list->get(&soundVolume);
}
