#pragma once

#include "dllUtilities.h"

struct Attribute_WeaponStats;

struct DLL_U Ammunition
{
	enum AmmunitionType
	{
		BULLET,
		SCATTER,
		EXPLOSIVE,

		NROFAMUNITIONTYPES
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
