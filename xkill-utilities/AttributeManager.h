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

	AttributeStorage<Attribute_Position>		position;
	AttributeStorage<Attribute_Spatial>			spatial;
	AttributeStorage<Attribute_Render>			render;
	AttributeStorage<Attribute_DebugShape>		debugShape;
	AttributeStorage<Attribute_Physics>			physics;
	AttributeStorage<Attribute_Camera>			camera;
	AttributeStorage<Attribute_Input>			input;
	AttributeStorage<Attribute_Player>			player;
	AttributeStorage<Attribute_Bounding>		bounding;
	AttributeStorage<Attribute_Projectile>		projectile;
	AttributeStorage<Attribute_Mesh>			mesh;
	AttributeStorage<Attribute_Health>			health;
	AttributeStorage<Attribute_Damage>			damage;
	AttributeStorage<Attribute_SpawnPoint>		spawnPoint;
	AttributeStorage<Attribute_WeaponStats>		weaponStats;
	AttributeStorage<Attribute_ExplosionSphere>	explosionSphere;

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