#include "AttributeManager.h"
#include "EntityStorage.h"

AttributeManager::AttributeManager()
{
	entities = new EntityStorage;

	position		.init	(ATTRIBUTE_POSITION);
	spatial			.init	(ATTRIBUTE_SPATIAL);
	render			.init	(ATTRIBUTE_RENDER);
	debugShape		.init	(ATTRIBUTE_DEBUGSHAPE);
	physics			.init	(ATTRIBUTE_PHYSICS);
	camera			.init	(ATTRIBUTE_CAMERA);
	splitScreen		.init	(ATTRIBUTE_SPLITSCREEN);
	input			.init	(ATTRIBUTE_INPUT);
	player			.init	(ATTRIBUTE_PLAYER);
	bounding		.init	(ATTRIBUTE_BOUNDING);
	projectile		.init	(ATTRIBUTE_PROJECTILE);
	lightDir		.init	(ATTRIBUTE_LIGHT_DIRECTIONAL);
	lightPoint		.init	(ATTRIBUTE_LIGHT_POINT);
	lightSpot		.init	(ATTRIBUTE_LIGHT_SPOT);
	mesh			.init	(ATTRIBUTE_MESH);
	health			.init	(ATTRIBUTE_HEALTH);
	damage			.init	(ATTRIBUTE_DAMAGE);
	spawnPoint		.init	(ATTRIBUTE_SPAWNPOINT);
	weaponStats		.init	(ATTRIBUTE_WEAPONSTATS);
	explosionSphere	.init	(ATTRIBUTE_EXPLOSIONSPHERE);
}

AttributeManager::~AttributeManager()
{
	delete entities;
}

AttributeManager* AttributeManager::getInstance()
{
	static AttributeManager instance;
	static bool test = true;
	if(test)
	{
		test = false;
	}
	return &instance;
}
