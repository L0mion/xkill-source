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

	AttributeStorage<PositionAttribute>			position;
	AttributeStorage<SpatialAttribute>			spatial;
	AttributeStorage<RenderAttribute>			render;
	AttributeStorage<DebugShapeAttribute>		debugShape;
	AttributeStorage<PhysicsAttribute>			physics;
	AttributeStorage<CameraAttribute>			camera;
	AttributeStorage<InputAttribute>			input;
	AttributeStorage<PlayerAttribute>			player;
	AttributeStorage<BoundingAttribute>			bounding;
	AttributeStorage<ProjectileAttribute>		projectile;
	AttributeStorage<MeshAttribute>				mesh;
	AttributeStorage<HealthAttribute>			health;
	AttributeStorage<DamageAttribute>			damage;
	AttributeStorage<SpawnPointAttribute>		spawnPoint;
	AttributeStorage<WeaponStatsAttribute>		weaponStats;
	AttributeStorage<ExplosionSphereAttribute>	explosionSphere;

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