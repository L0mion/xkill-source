#pragma once

#include "dllUtilities.h"
#include "XKILL_Enums.h"

struct Attribute_WeaponStats;

//! Stores all data related to ammunition
struct DLL_U Ammunition
{
	unsigned int maxTotalNrOfShots;			//!< The maximum number of shots that can be carried of this type of ammunition.
	unsigned int initialTotalNrOfShots;		//!< Number of shots a player spawns with.
	unsigned int currentTotalNrOfShots;		//!< Current amount of total number of shots.
	float speed;							//!< Velocity of the PhysicsAttribute when creating a projectile.
	float damage;							//!< Damage value of the damage attribute of each projectile created when creating a projectile from this weapon.
	unsigned int nrOfProjectilesPerSalvo;	//!< If > 1 then scattershot else singleshot.
	float spreadConeRadius;					//!< Randomizes the orientation of each projectile's velocity vector inside this cone.
	float velocityVariation;				//!< Randomizes the speed of a projectile around a given range. If velocityVariation is 4, speed will be randomized in the range 2<->6. If 1, randomization range till be 0.5<->1.5.
	float displacementSphereRadius;			//!< Randomizes the position of each projectile inside this sphere.
	bool canShootWhileReloading;			//!< Can you shoot halfway through reloading this ammunition?
	bool isReloading;						//!< Are we reloading the weapon?

	bool explosive;							//!< Determines if projectiles created from this weapon will explode on impact.
	float explosionSphereInitialRadius;		//!< The initial radius of an explosion sphere created from this projectile
	float explosionSphereFinalRadius;		//!< The final radius of an explosion sphere created from this projectile
	float explosionSphereExplosionDuration;	//!< The life time of an explosion sphere created from this projectile

	XKILL_Enums::AmmunitionType type;
};

//! Stores all data related to firing mode
struct DLL_U FiringMode
{
	bool canShootBullet;
	bool canShootScatter;
	bool canShootExplosive;

	unsigned int clipSize;				//!< Number of shots that can be fired before reload is needed. A value of 0 denotes that no reload is necessary.
	float reloadTime;					//!< Number of seconds it takes to reload.
	float cooldownBetweenShots;			//!< Number of seconds that must pass between each shot.
	float damageModifier;
	float explosionSphereModifier;

	unsigned int nrOfShotsLeftInClip[XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES];	//!< Current number of shots left in the current clip for each ammunition.
	float reloadTimeLeft;				//!< Reload progress, when lesser or equal to 0 the weapon is reloaded.
	float cooldownLeft;					//!< Number of seconds until a new shot can be fired.

	XKILL_Enums::FiringModeType type;
};
