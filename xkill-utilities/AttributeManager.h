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
	AttributeManager()
	{
		positionAttributes			.init(ATTRIBUTE_POSITION);
		spatialAttributes			.init(ATTRIBUTE_SPATIAL);
		renderAttributes			.init(ATTRIBUTE_RENDER);
		debugShapeAttributes		.init(ATTRIBUTE_DEBUGSHAPE);
		physicsAttributes			.init(ATTRIBUTE_PHYSICS);
		cameraAttributes			.init(ATTRIBUTE_CAMERA);
		inputAttributes				.init(ATTRIBUTE_INPUT);
		playerAttributes			.init(ATTRIBUTE_PLAYER);
		boundingAttributes			.init(ATTRIBUTE_BOUNDING);
		meshAttributes				.init(ATTRIBUTE_MESH);
		projectileAttributes		.init(ATTRIBUTE_PROJECTILE);

		healthAttributes			.init(ATTRIBUTE_HEALTH);
		damageAttributes			.init(ATTRIBUTE_DAMAGE);
		spawnPointAttributes		.init(ATTRIBUTE_SPAWNPOINT);
		weaponStatsAttributes		.init(ATTRIBUTE_WEAPONSTATS);
		explosionSphereAttributes	.init(ATTRIBUTE_EXPLOSIONSPHERE);
	}

public:
	AttributeStorage<PositionAttribute>			positionAttributes;
	AttributeStorage<SpatialAttribute>			spatialAttributes;
	AttributeStorage<RenderAttribute>			renderAttributes;
	AttributeStorage<DebugShapeAttribute>		debugShapeAttributes;
	AttributeStorage<PhysicsAttribute>			physicsAttributes;
	AttributeStorage<CameraAttribute>			cameraAttributes;
	AttributeStorage<InputAttribute>			inputAttributes;
	AttributeStorage<PlayerAttribute>			playerAttributes;
	AttributeStorage<BoundingAttribute>			boundingAttributes;
	AttributeStorage<ProjectileAttribute>		projectileAttributes;
	AttributeStorage<MeshAttribute>				meshAttributes;
	AttributeStorage<HealthAttribute>			healthAttributes;
	AttributeStorage<DamageAttribute>			damageAttributes;
	AttributeStorage<SpawnPointAttribute>		spawnPointAttributes;
	AttributeStorage<WeaponStatsAttribute>		weaponStatsAttributes;
	AttributeStorage<ExplosionSphereAttribute>	explosionSphereAttributes;

	static AttributeManager* getInstance()
	{
		static AttributeManager instance;
		static bool test = true;
		if(test)
		{
			test = false;
		}
		return &instance;
	}
};


class DLL_U InstanceTest
{
public:
	static InstanceTest* getInstance()
	{
		static InstanceTest instance;
		static bool test = true;
		if(test)
		{
			test = false;
		}
		return &instance;
	}
};