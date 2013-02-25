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
	numErrors = 0;
	soundVolume = 0.1f;
	numPlayers = 2;
	_timeScale = 1.0f;
	trueDeltaTime = 0.0f;
	currentLevel = "TestArena";
	timeLimit = 0.0f;
	cycleLimit = 35;
	showDebugPhysics = false;
}
