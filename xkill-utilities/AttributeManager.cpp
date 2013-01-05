#include "AttributeManager.h"

AttributeManager::AttributeManager()
{
	position		.init	(ATTRIBUTE_POSITION);
	spatial			.init	(ATTRIBUTE_SPATIAL);
	render			.init	(ATTRIBUTE_RENDER);
	debugShape		.init	(ATTRIBUTE_DEBUGSHAPE);
	physics			.init	(ATTRIBUTE_PHYSICS);
	camera			.init	(ATTRIBUTE_CAMERA);
	input			.init	(ATTRIBUTE_INPUT);
	player			.init	(ATTRIBUTE_PLAYER);
	bounding		.init	(ATTRIBUTE_BOUNDING);
	projectile		.init	(ATTRIBUTE_PROJECTILE);			
	mesh			.init	(ATTRIBUTE_MESH);
	health			.init	(ATTRIBUTE_HEALTH);
	damage			.init	(ATTRIBUTE_DAMAGE);
	spawnPoint		.init	(ATTRIBUTE_SPAWNPOINT);
	weaponStats		.init	(ATTRIBUTE_WEAPONSTATS);
	explosionSphere	.init	(ATTRIBUTE_EXPLOSIONSPHERE);
}

AttributeManager::~AttributeManager()
{
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
