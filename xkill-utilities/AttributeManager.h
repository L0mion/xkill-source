#pragma once

#include "AttributeType.h"
#include "AttributeStorage.h"

class EntityStorage;
//static Entity* settings_entity;

// Settings class
class DLL_U Settings
{
private:
	float _timeScale;

public:
	int numPlayers;
	bool showDebugPhysics;
	std::string currentLevel;
	float timeLimit;
	int cycleLimit;
	float trueDeltaTime;
	bool soundMuted;
	float soundVolume;

	float timeScale();
	void setTimeScale(float timeScale);

	Settings()
	{
		numPlayers = 2;
		_timeScale = 1.0f;
		trueDeltaTime = 0.0f;
		currentLevel = "TestArena";
		timeLimit = 0.0f;
		cycleLimit = 35;
		showDebugPhysics = false;
	}
};



/// Singleton responsible for providing access to each Attribute.
/** 
\ingroup ARCHITECTURE
*/


class DLL_U AttributeManager
{
private:
	AttributeManager()
	{
		createEntityStorage();
		settings = new Settings;
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
	AttributeStorage<Attribute_InputDevice>				inputDevice;
	AttributeStorage<Attribute_Player>					player;
	AttributeStorage<Attribute_Bounding>				bounding;
	AttributeStorage<Attribute_Projectile>				projectile;
	AttributeStorage<Attribute_Light_Dir>				lightDir;
	AttributeStorage<Attribute_Light_Point>				lightPoint;
	AttributeStorage<Attribute_Light_Spot>				lightSpot;
	AttributeStorage<Attribute_Mesh>					mesh;
	AttributeStorage<Attribute_Health>					health;
	AttributeStorage<Attribute_Damage>					damage;
	AttributeStorage<Attribute_PlayerSpawnPoint>		playerSpawnPoint;
	AttributeStorage<Attribute_WeaponStats>				weaponStats;
	AttributeStorage<Attribute_ExplosionSphere>			explosionSphere;
	AttributeStorage<Attribute_SplitScreen>				splitScreen;
	AttributeStorage<Attribute_PickupablesSpawnPoint>	pickupablesSpawnPoint;
	AttributeStorage<Attribute_Pickupable>				pickupable;
	AttributeStorage<Attribute_SoundSettings>			soundSettings;
	AttributeStorage<Attribute_Ray>						ray;

	AttributeStorage<Behavior_Offset>					offset;

	// ADD MORE ABOVE ^

	static AttributeManager* instance();
};

#define SETTINGS															\
AttributeManager::instance()->settings

// Declares all attributes
#define ATTRIBUTES_DECLARE_ALL															\
	static Settings												*settings				;	\
	static EntityStorage										*itr_entity				;	\
	static AttributeIterator<Attribute_Position>				itrPosition				;	\
	static AttributeIterator<Attribute_Spatial>					itrSpatial				;	\
	static AttributeIterator<Attribute_Render>					itrRender				;	\
	static AttributeIterator<Attribute_DebugShape>				itrDebugShape			;	\
	static AttributeIterator<Attribute_Physics>					itrPhysics				;	\
	static AttributeIterator<Attribute_Camera>					itrCamera				;	\
	static AttributeIterator<Attribute_Input>					itrInput				;	\
	static AttributeIterator<Attribute_InputDevice>				itrInputDevice			;	\
	static AttributeIterator<Attribute_Player>					itrPlayer				;	\
	static AttributeIterator<Attribute_Bounding>				itrBounding				;	\
	static AttributeIterator<Attribute_Projectile>				itrProjectile			;	\
	static AttributeIterator<Attribute_Light_Dir>				itrLightDir				;	\
	static AttributeIterator<Attribute_Light_Point>				itrLightPoint			;	\
	static AttributeIterator<Attribute_Light_Spot>				itrLightSpot			;	\
	static AttributeIterator<Attribute_Mesh>					itrMesh					;	\
	static AttributeIterator<Attribute_Health>					itrHealth				;	\
	static AttributeIterator<Attribute_Damage>					itrDamage				;	\
	static AttributeIterator<Attribute_PlayerSpawnPoint>		itrPlayerSpawnPoint		;	\
	static AttributeIterator<Attribute_WeaponStats>				itrWeaponStats			;	\
	static AttributeIterator<Attribute_ExplosionSphere>			itrExplosionSphere		;	\
	static AttributeIterator<Attribute_SplitScreen>				itrSplitScreen			;	\
	static AttributeIterator<Attribute_PickupablesSpawnPoint>	itrPickupablesSpawnPoint;	\
	static AttributeIterator<Attribute_Pickupable>				itrPickupable;			;	\
	static AttributeIterator<Attribute_SoundSettings>			itrSoundSettings		;	\
	static AttributeIterator<Attribute_Ray>						itrRay;					;	\
	\
	static AttributeIterator<Behavior_Offset>					itrOffset				;	\
	\
	// ADD MORE ABOVE ^

// Inits all attributes
#define ATTRIBUTES_INIT_ALL																					\
	settings				= AttributeManager::instance()->settings;										\
	itr_entity				= AttributeManager::instance()->entities;										\
	\
	itrPosition				= AttributeManager::instance()->position					.getIterator();		\
	itrSpatial				= AttributeManager::instance()->spatial						.getIterator();		\
	itrRender				= AttributeManager::instance()->render						.getIterator();		\
	itrDebugShape			= AttributeManager::instance()->debugShape					.getIterator();		\
	itrPhysics				= AttributeManager::instance()->physics						.getIterator();		\
	itrCamera				= AttributeManager::instance()->camera						.getIterator();		\
	itrInput				= AttributeManager::instance()->input						.getIterator();		\
	itrInputDevice			= AttributeManager::instance()->inputDevice					.getIterator();		\
	itrPlayer				= AttributeManager::instance()->player						.getIterator();		\
	itrBounding				= AttributeManager::instance()->bounding					.getIterator();		\
	itrProjectile			= AttributeManager::instance()->projectile					.getIterator();		\
	itrLightDir				= AttributeManager::instance()->lightDir					.getIterator();		\
	itrLightPoint			= AttributeManager::instance()->lightPoint					.getIterator();		\
	itrLightSpot			= AttributeManager::instance()->lightSpot					.getIterator();		\
	itrMesh					= AttributeManager::instance()->mesh						.getIterator();		\
	itrHealth				= AttributeManager::instance()->health						.getIterator();		\
	itrDamage				= AttributeManager::instance()->damage						.getIterator();		\
	itrPlayerSpawnPoint		= AttributeManager::instance()->playerSpawnPoint			.getIterator();		\
	itrWeaponStats			= AttributeManager::instance()->weaponStats					.getIterator();		\
	itrExplosionSphere		= AttributeManager::instance()->explosionSphere				.getIterator();		\
	itrSplitScreen			= AttributeManager::instance()->splitScreen					.getIterator();		\
	itrPickupablesSpawnPoint= AttributeManager::instance()->pickupablesSpawnPoint		.getIterator();		\
	itrPickupable			= AttributeManager::instance()->pickupable					.getIterator();		\
	itrSoundSettings		= AttributeManager::instance()->soundSettings				.getIterator();		\
	itrRay					= AttributeManager::instance()->ray							.getIterator();		\
	\
	itrOffset				= AttributeManager::instance()->offset						.getIterator();		\
	\
	// ADD MORE ABOVE ^
