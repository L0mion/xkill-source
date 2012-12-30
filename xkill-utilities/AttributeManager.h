#pragma once

#include "AttributeType.h"
#include "AttributeStorage.h"

/// Singleton responsible for providing access to each Attribute.
/** 
\ingroup ARCHITECTURE
*/

class AttributeManager
{
private:
	AttributeManager();

public:
	~AttributeManager();

	AttributeStorage<PositionAttribute>			*positionAttributes;
	AttributeStorage<SpatialAttribute>			*spatialAttributes;
	AttributeStorage<RenderAttribute>			*renderAttributes;
	AttributeStorage<DebugShapeAttribute>		*debugShapeAttributes;
	AttributeStorage<PhysicsAttribute>			*physicsAttributes;
	AttributeStorage<CameraAttribute>			*cameraAttributes;
	AttributeStorage<InputAttribute>			*inputAttributes;
	AttributeStorage<PlayerAttribute>			*playerAttributes;
	AttributeStorage<BoundingAttribute>			*boundingAttributes;
	AttributeStorage<ProjectileAttribute>		*projectileAttributes;
	AttributeStorage<MeshAttribute>				*meshAttributes;
	AttributeStorage<HealthAttribute>			*healthAttributes;
	AttributeStorage<DamageAttribute>			*damageAttributes;
	AttributeStorage<SpawnPointAttribute>		*spawnPointAttributes;
	AttributeStorage<WeaponStatsAttribute>		*weaponStatsAttributes;
	AttributeStorage<ExplosionSphereAttribute>	*explosionSphereAttributes;

	static AttributeManager* getInstance();
};


class DLL_U AttributeManagerDLLWrapper
{
public:
	static void* getInstance()
	{
		static void* instance =  AttributeManager::getInstance();
		static bool test = true;
		if(test)
		{
			test = false;
		}
		return instance;
	}
};