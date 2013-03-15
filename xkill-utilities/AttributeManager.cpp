#include "AttributeManager.h"
#include "EntityStorage.h"
#include "DataItem.h"



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

AttributeManager::AttributeManager()
{
	// Create entity storage
	entities = new EntityStorage;
	
	// Create settings attribute which holds all in game settings
	settings.createAttribute(entities->createEntity());
}

float Attribute_Settings::timeScale()
{
	return _timeScale;
}

void Attribute_Settings::setTimeScale( float timeScale )
{
	_timeScale = timeScale;

	if(_timeScale > 2.0f)
		_timeScale = 2.0f;

	if(_timeScale < 0.01f)
		_timeScale = 0.01f;
}

Attribute_Settings::Attribute_Settings()
{
	timeUntilScheduling = 0.0f;
	numErrors = 0;
	soundVolume = 0.1f;
	numPlayers = 1;
	_timeScale = 1.0f;
	trueDeltaTime = 0.0f;
	currentLevel = "TestArena";
	timeLimit = 0.0f;
	timeLeft = 0.0f;
	cycleLimit = 35;
	schedulerTime = 30.0f;
	cycleTime = 2.5f;
	showDebugPhysics = false;
	respawnTime = 3;
	nullprocessExecutionTime = 30;
	isNullprocessExecuting = false;
	averageDeltaTime = 0.0f;
	trueDeltaTime = 0.0f;

	occlusionRadius		= 0.2f;
	occlusionScale		= 0.7f;
	occlusionBias		= 0.1f;
	occlusionIntensity	= 2.0f;
}

DataItemList* Attribute_Settings::getDataList()
{
	DataItemList* list = new DataItemList();

	list->add(soundVolume, "soundVolume");
	list->add(occlusionRadius, "SSAO_occlusionRadius");
	list->add(occlusionScale, "SSAO_occlusionScale");
	list->add(occlusionBias, "SSAO_occlusionBias");
	list->add(occlusionIntensity, "SSAO_occlusionIntensity");

	return list;
}

void Attribute_Settings::saveTo( DataItemList* list )
{
	list->get(&soundVolume);

	list->get(&occlusionRadius);
	list->get(&occlusionScale);
	list->get(&occlusionBias);
	list->get(&occlusionIntensity);
}
