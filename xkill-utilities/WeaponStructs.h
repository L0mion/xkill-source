#pragma once

#include "dllUtilities.h"
#include "XKILL_Enums.h"

struct Attribute_WeaponStats;

//! Stores all data related to ammunition
struct DLL_U Ammunition
{
	unsigned int totalNrOfShots; 
	float speed;
	float damage;
	unsigned int nrOfProjectiles;
	float spread;
	float velocityVariation;
	float spawnVariation;

	bool explosive;
	float explosionSphereInitialRadius;
	float explosionSphereFinalRadius;
	float explosionSphereExplosionDuration;

	XKILL_Enums::AmmunitionType type;
};

//! Stores all data related to firing mode
struct DLL_U FiringMode
{
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

	XKILL_Enums::FiringModeType type;
};
