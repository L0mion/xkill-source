#include "MutatorSettings.h"

#include "AttributeType.h"

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

Ammunition	MutatorSettings::standardAmmunitions[Ammunition::NROFAMMUNITIONTYPES];
FiringMode	MutatorSettings::standardFiringModes[FiringMode::NROFFIRINGMODETYPES];
bool		MutatorSettings::hasInitialized = false;

MutatorSettings::MutatorSettings()
{
	if(!hasInitialized)
	{
		initStandardAmmunition();
		initStandardFiringModes();

		hasInitialized = true;
	}
}

MutatorSettings::~MutatorSettings()
{

}

void MutatorSettings::setupAttribute(Attribute_WeaponStats* attribute)
{
	setupAttribute(attribute, attribute->currentAmmunitionType, attribute->currentFiringModeType);
}

void MutatorSettings::setupAttribute(Attribute_WeaponStats* attribute, Ammunition::AmmunitionType ammo, FiringMode::FiringModeType firingMode)
{
	attribute->ammunition[ammo] = Ammunition(standardAmmunitions[ammo]);
	attribute->firingMode[firingMode] = FiringMode(standardFiringModes[firingMode]);
}

Ammunition& MutatorSettings::getStandardAmmunition(Ammunition::AmmunitionType type)
{
	return standardAmmunitions[type];
}

FiringMode& MutatorSettings::getStandardFiringMode(FiringMode::FiringModeType type)
{
	return standardFiringModes[type];
}

void MutatorSettings::initStandardAmmunition()
{
	Ammunition* ammo;

	ammo = &standardAmmunitions[Ammunition::BULLET];

	ammo->type				= Ammunition::BULLET;
	ammo->damage			= 5.0f;
	ammo->explosionSphere	= 1.0f;
	ammo->explosive			= false;
	ammo->nrOfProjectiles	= 1;
	ammo->spawnVariation	= 0.0f;
	ammo->speed				= 40.0f;
	ammo->spread			= 0.0f;
	ammo->totalNrOfShots	= 1000;
	ammo->velocityVariation	= 0.0f;

	ammo = &standardAmmunitions[Ammunition::SCATTER];

	ammo->type				= Ammunition::SCATTER;
	ammo->damage			= 2.0f;
	ammo->explosionSphere	= 1.0f;
	ammo->explosive			= false;
	ammo->nrOfProjectiles	= 10;
	ammo->spawnVariation	= 0.02f;
	ammo->speed				= 10.0f;
	ammo->spread			= 0.2f;
	ammo->totalNrOfShots	= 1000;
	ammo->velocityVariation	= 0.5f;

	ammo = &standardAmmunitions[Ammunition::EXPLOSIVE];

	ammo->type				= Ammunition::EXPLOSIVE;
	ammo->damage			= 10.0f;
	ammo->explosionSphere	= 1.0f;
	ammo->explosive			= true;
	ammo->nrOfProjectiles	= 1;
	ammo->spawnVariation	= 0.0f;
	ammo->speed				= 5.0f;
	ammo->spread			= 0.0f;
	ammo->totalNrOfShots	= 1000;
	ammo->velocityVariation	= 0.0f;
}

void MutatorSettings::initStandardFiringModes()
{
	FiringMode* firingMode;

	firingMode = &standardFiringModes[FiringMode::SINGLE];

	firingMode->type					= FiringMode::SINGLE;
	firingMode->canShootBullet			= true;
	firingMode->canShootExplosive		= true;
	firingMode->canShootScatter			= true;
	firingMode->clipSize				= 1;
	firingMode->damageModifier			= 1.0f;
	firingMode->explosionSphereModifier	= 1.0f;
	firingMode->cooldownBetweenShots	= 0.0f;
	firingMode->reloadTime				= 1.0f;
	firingMode->nrOfShotsLeftInClip		= firingMode->clipSize;
	firingMode->cooldownLeft			= firingMode->cooldownBetweenShots;
	firingMode->reloadTimeLeft			= firingMode->reloadTime;

	firingMode = &standardFiringModes[FiringMode::SEMI];

	firingMode->type					= FiringMode::SEMI;
	firingMode->canShootBullet			= true;
	firingMode->canShootExplosive		= true;
	firingMode->canShootScatter			= true;
	firingMode->clipSize				= 10;
	firingMode->damageModifier			= 1.0f;
	firingMode->explosionSphereModifier	= 0.5f;
	firingMode->cooldownBetweenShots	= 0.5f;
	firingMode->reloadTime				= 2.0f;
	firingMode->nrOfShotsLeftInClip		= firingMode->clipSize;
	firingMode->cooldownLeft			= firingMode->cooldownBetweenShots;
	firingMode->reloadTimeLeft			= firingMode->reloadTime;

	firingMode = &standardFiringModes[FiringMode::AUTO];

	firingMode->type					= FiringMode::AUTO;
	firingMode->canShootBullet			= true;
	firingMode->canShootExplosive		= true;
	firingMode->canShootScatter			= true;
	firingMode->clipSize				= 50;
	firingMode->damageModifier			= 1.0f;
	firingMode->explosionSphereModifier	= 0.15f;
	firingMode->cooldownBetweenShots	= 0.1f;
	firingMode->reloadTime				= 3.0f;
	firingMode->nrOfShotsLeftInClip		= firingMode->clipSize;
	firingMode->cooldownLeft			= firingMode->cooldownBetweenShots;
	firingMode->reloadTimeLeft			= firingMode->reloadTime;
}