#pragma once

#include "AttributeType.h"
#include "AttributeStorage.h"

class EntityStorage;
//static Entity* settings_entity;

// Settings class
class DLL_U Settings
{
public:
	float timeScale;

	Settings()
	{
		timeScale = 1.0f;
	}
};



/// Singleton responsible for providing access to each Attribute.
/** 
\ingroup ARCHITECTURE
*/


class AttributeManager
{
private:
	AttributeManager()
	{
		createEntityStorage();
		settings = new Settings;

		position		.init	(ATTRIBUTE_POSITION);
		spatial			.init	(ATTRIBUTE_SPATIAL);
		render			.init	(ATTRIBUTE_RENDER);
		debugShape		.init	(ATTRIBUTE_DEBUGSHAPE);
		physics			.init	(ATTRIBUTE_PHYSICS);
		camera			.init	(ATTRIBUTE_CAMERA);
		input			.init	(ATTRIBUTE_INPUT);
		//inputDeviceSettings	.init	(ATTRIBUTE_INPUTDEVICESETTINGS);
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

		// ADD MORE ABOVE ^

	}

	void createEntityStorage();

public:
	~AttributeManager();

	Settings* settings;
	EntityStorage* entities;

	AttributeStorage<Attribute_Position>				position;
	AttributeStorage<Attribute_Spatial>					spatial;
	AttributeStorage<Attribute_Render>					render;
	AttributeStorage<Attribute_DebugShape>				debugShape;
	AttributeStorage<Attribute_Physics>					physics;
	AttributeStorage<Attribute_Camera>					camera;
	AttributeStorage<Attribute_Input>					input;
	//AttributeStorage<Attribute_InputDeviceSettings>		inputDeviceSettings;
	AttributeStorage<Attribute_Player>					player;
	AttributeStorage<Attribute_Bounding>				bounding;
	AttributeStorage<Attribute_Projectile>				projectile;
	AttributeStorage<Attribute_Light_Dir>				lightDir;
	AttributeStorage<Attribute_Light_Point>				lightPoint;
	AttributeStorage<Attribute_Light_Spot>				lightSpot;
	AttributeStorage<Attribute_Mesh>					mesh;
	AttributeStorage<Attribute_Health>					health;
	AttributeStorage<Attribute_Damage>					damage;
	AttributeStorage<Attribute_SpawnPoint>				spawnPoint;
	AttributeStorage<Attribute_WeaponStats>				weaponStats;
	AttributeStorage<Attribute_ExplosionSphere>			explosionSphere;

	// ADD MORE ABOVE ^


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
	static Settings settingsx;
};


// Declares all attributes
#define ATTRIBUTES_DECLARE_ALL														\
static	Settings											*settings				;	\
static EntityStorage										*entityStorage			;	\
static AttributeIterator<Attribute_Position>				itrPosition				;	\
static AttributeIterator<Attribute_Spatial>				itrSpatial				;	\
static AttributeIterator<Attribute_Render>					itrRender				;	\
static AttributeIterator<Attribute_DebugShape>				itrDebugShape			;	\
static AttributeIterator<Attribute_Physics>				itrPhysics				;	\
static AttributeIterator<Attribute_Camera>					itrCamera				;	\
static AttributeIterator<Attribute_Input>					itrInput				;	\
/*static	AttributeIterator<Attribute_InputDeviceSettings>	itrInputDeviceSettings	;	*/\
static AttributeIterator<Attribute_Player>					itrPlayer				;	\
static AttributeIterator<Attribute_Bounding>				itrBounding				;	\
static AttributeIterator<Attribute_Projectile>				itrProjectile			;	\
static AttributeIterator<Attribute_Light_Dir>				itrLightDir				;	\
static AttributeIterator<Attribute_Light_Point>			itrLightPoint			;	\
static AttributeIterator<Attribute_Light_Spot>				itrLightSpot			;	\
static AttributeIterator<Attribute_Mesh>					itrMesh					;	\
static AttributeIterator<Attribute_Health>					itrHealth				;	\
static AttributeIterator<Attribute_Damage>					itrDamage				;	\
static AttributeIterator<Attribute_SpawnPoint>				itrSpawnPoint			;	\
static AttributeIterator<Attribute_WeaponStats>			itrWeaponStats			;	\
static AttributeIterator<Attribute_ExplosionSphere>		itrExplosionSphere		;	\
																						\
// ADD MORE ABOVE ^

// Inits all attributes
#define ATTRIBUTES_INIT_ALL																\
settings				= ATTRIBUTE_MANAGER->settings;									\
entityStorage			= ATTRIBUTE_MANAGER->entities;									\
itrPosition				= ATTRIBUTE_MANAGER->position				.getIterator();		\
itrSpatial				= ATTRIBUTE_MANAGER->spatial				.getIterator();		\
itrRender				= ATTRIBUTE_MANAGER->render					.getIterator();		\
itrDebugShape			= ATTRIBUTE_MANAGER->debugShape				.getIterator();		\
itrPhysics				= ATTRIBUTE_MANAGER->physics				.getIterator();		\
itrCamera				= ATTRIBUTE_MANAGER->camera					.getIterator();		\
itrInput				= ATTRIBUTE_MANAGER->input					.getIterator();		\
/*itrInputDeviceSettings	= ATTRIBUTE_MANAGER->inputDeviceSettings	.getIterator();		*/\
itrPlayer				= ATTRIBUTE_MANAGER->player					.getIterator();		\
itrBounding				= ATTRIBUTE_MANAGER->bounding				.getIterator();		\
itrProjectile			= ATTRIBUTE_MANAGER->projectile				.getIterator();		\
itrLightDir				= ATTRIBUTE_MANAGER->lightDir				.getIterator();		\
itrLightPoint			= ATTRIBUTE_MANAGER->lightPoint				.getIterator();		\
itrLightSpot			= ATTRIBUTE_MANAGER->lightSpot				.getIterator();		\
itrMesh					= ATTRIBUTE_MANAGER->mesh					.getIterator();		\
itrHealth				= ATTRIBUTE_MANAGER->health					.getIterator();		\
itrDamage				= ATTRIBUTE_MANAGER->damage					.getIterator();		\
itrSpawnPoint			= ATTRIBUTE_MANAGER->spawnPoint				.getIterator();		\
itrWeaponStats			= ATTRIBUTE_MANAGER->weaponStats			.getIterator();		\
itrExplosionSphere		= ATTRIBUTE_MANAGER->explosionSphere		.getIterator();		\
																						\
// ADD MORE ABOVE ^
