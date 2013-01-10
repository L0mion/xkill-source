#pragma once

#include "AttributeType.h"
#include "AttributeStorage.h"

class EntityStorage;

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

	EntityStorage* entities;

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
	AttributeStorage<Attribute_Light_Dir>	lightDir;
	AttributeStorage<Attribute_Light_Point>			lightPoint;
	AttributeStorage<Attribute_Light_Spot>			lightSpot;
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


// Declares all attributes
#define ATTRIBUTES_DECLARE_ALL														\
static	EntityStorage										*entityStorage		;	\
static	AttributeIterator<Attribute_Position>				itrPosition			;	\
static	AttributeIterator<Attribute_Spatial>				itrSpatial			;	\
static	AttributeIterator<Attribute_Render>					itrRender			;	\
static	AttributeIterator<Attribute_DebugShape>				itrDebugShape		;	\
static	AttributeIterator<Attribute_Physics>				itrPhysics			;	\
static	AttributeIterator<Attribute_Camera>					itrCamera			;	\
static	AttributeIterator<Attribute_Input>					itrInput			;	\
static	AttributeIterator<Attribute_Player>					itrPlayer			;	\
static	AttributeIterator<Attribute_Bounding>				itrBounding			;	\
static	AttributeIterator<Attribute_Projectile>				itrProjectile		;	\
static	AttributeIterator<Attribute_Light_Dir>		itrLightDir			;	\
static	AttributeIterator<Attribute_Light_Point>			itrLightPoint		;	\
static	AttributeIterator<Attribute_Light_Spot>				itrLightSpot		;	\
static	AttributeIterator<Attribute_Mesh>					itrMesh				;	\
static	AttributeIterator<Attribute_Health>					itrHealth			;	\
static	AttributeIterator<Attribute_Damage>					itrDamage			;	\
static	AttributeIterator<Attribute_SpawnPoint>				itrSpawnPoint		;	\
static	AttributeIterator<Attribute_WeaponStats>			itrWeaponStats		;	\
static	AttributeIterator<Attribute_ExplosionSphere>		itrExplosionSphere	;	\
																					\
// ADD MORE ABOVE ^

// Inits all attributes
#define ATTRIBUTES_INIT_ALL															\
entityStorage		= ATTRIBUTE_MANAGER->entities;									\
itrPosition			= ATTRIBUTE_MANAGER->position			.getIterator();			\
itrSpatial			= ATTRIBUTE_MANAGER->spatial			.getIterator();			\
itrRender			= ATTRIBUTE_MANAGER->render				.getIterator();			\
itrDebugShape		= ATTRIBUTE_MANAGER->debugShape			.getIterator();			\
itrPhysics			= ATTRIBUTE_MANAGER->physics			.getIterator();			\
itrCamera			= ATTRIBUTE_MANAGER->camera				.getIterator();			\
itrInput			= ATTRIBUTE_MANAGER->input				.getIterator();			\
itrPlayer			= ATTRIBUTE_MANAGER->player				.getIterator();			\
itrBounding			= ATTRIBUTE_MANAGER->bounding			.getIterator();			\
itrProjectile		= ATTRIBUTE_MANAGER->projectile			.getIterator();			\
itrLightDir			= ATTRIBUTE_MANAGER->lightDir			.getIterator();			\
itrLightPoint		= ATTRIBUTE_MANAGER->lightPoint			.getIterator();			\
itrLightSpot		= ATTRIBUTE_MANAGER->lightSpot			.getIterator();			\
itrMesh				= ATTRIBUTE_MANAGER->mesh				.getIterator();			\
itrHealth			= ATTRIBUTE_MANAGER->health				.getIterator();			\
itrDamage			= ATTRIBUTE_MANAGER->damage				.getIterator();			\
itrSpawnPoint		= ATTRIBUTE_MANAGER->spawnPoint			.getIterator();			\
itrWeaponStats		= ATTRIBUTE_MANAGER->weaponStats		.getIterator();			\
itrExplosionSphere	= ATTRIBUTE_MANAGER->explosionSphere	.getIterator();			\
																					\
// ADD MORE ABOVE ^
