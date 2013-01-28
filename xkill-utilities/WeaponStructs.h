#pragma once

#include "dllUtilities.h"

struct Attribute_WeaponStats;

//! Stores all data related to ammunition
struct DLL_U Ammunition
{
	enum AmmunitionType
	{
		BULLET,
		SCATTER,
		EXPLOSIVE,

		NROFAMMUNITIONTYPES
	};

	unsigned int totalNrOfShots; 
	float speed;
	float damage;
	unsigned int nrOfProjectiles;
	float spread;
	float velocityVariation;
	float spawnVariation;
	bool explosive;
	float explosionSphere;

	AmmunitionType type;
};

//! Stores all data related to firing mode
struct DLL_U FiringMode
{
	enum FiringModeType
	{
		SINGLE,
		SEMI,
		AUTO,

		NROFFIRINGMODETYPES
	};

	bool canShootBullet;
	bool canShootScatter;
	bool canShootExplosive;

	unsigned int clipSize;
	float reloadTime;
	float cooldownBetweenShots;
	float damageModifier;
	float explosionSphereModifier;

	unsigned int nrOfShotsLeftInClip;
	float reloadTimeLeft;
	float cooldownLeft;

	FiringModeType type;
};
