#include "AttributeManager.h"

AttributeManager::AttributeManager()
{
	positionAttributes			= new AttributeStorage<PositionAttribute>			(ATTRIBUTE_POSITION);
	spatialAttributes			= new AttributeStorage<SpatialAttribute>			(ATTRIBUTE_SPATIAL);
	renderAttributes			= new AttributeStorage<RenderAttribute>				(ATTRIBUTE_RENDER);
	debugShapeAttributes		= new AttributeStorage<DebugShapeAttribute>			(ATTRIBUTE_DEBUGSHAPE);
	physicsAttributes			= new AttributeStorage<PhysicsAttribute>			(ATTRIBUTE_PHYSICS);
	cameraAttributes			= new AttributeStorage<CameraAttribute>				(ATTRIBUTE_CAMERA);
	inputAttributes				= new AttributeStorage<InputAttribute>				(ATTRIBUTE_INPUT);
	playerAttributes			= new AttributeStorage<PlayerAttribute>				(ATTRIBUTE_PLAYER);
	boundingAttributes			= new AttributeStorage<BoundingAttribute>			(ATTRIBUTE_BOUNDING);
	projectileAttributes		= new AttributeStorage<ProjectileAttribute>			(ATTRIBUTE_PROJECTILE);			
	meshAttributes				= new AttributeStorage<MeshAttribute>				(ATTRIBUTE_MESH);
	healthAttributes			= new AttributeStorage<HealthAttribute>				(ATTRIBUTE_HEALTH);
	damageAttributes			= new AttributeStorage<DamageAttribute>				(ATTRIBUTE_DAMAGE);
	spawnPointAttributes		= new AttributeStorage<SpawnPointAttribute>			(ATTRIBUTE_SPAWNPOINT);
	weaponStatsAttributes		= new AttributeStorage<WeaponStatsAttribute>		(ATTRIBUTE_WEAPONSTATS);
	explosionSphereAttributes	= new AttributeStorage<ExplosionSphereAttribute>	(ATTRIBUTE_EXPLOSIONSPHERE);
}

AttributeManager::~AttributeManager()
{
	positionAttributes			;
	spatialAttributes			;
	renderAttributes			;
	debugShapeAttributes		;
	physicsAttributes			;
	cameraAttributes			;
	inputAttributes				;
	playerAttributes			;
	boundingAttributes			;
	projectileAttributes		;
	meshAttributes				;
	healthAttributes			;
	damageAttributes			;
	spawnPointAttributes		;
	weaponStatsAttributes		;
	explosionSphereAttributes	;
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
