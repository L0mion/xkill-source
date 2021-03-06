#include "MutatorSettings.h"

#include "AttributeType.h"

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

Ammunition	MutatorSettings::standardAmmunitions[XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES];
FiringMode	MutatorSettings::standardFiringModes[XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES];
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

void MutatorSettings::setupAttribute(AttributePtr<Attribute_WeaponStats> attribute_ptr)
{
	setupAttribute(attribute_ptr, attribute_ptr->currentAmmunitionType, attribute_ptr->currentFiringModeType);
}

void MutatorSettings::setupAttribute(AttributePtr<Attribute_WeaponStats> attribute_ptr, XKILL_Enums::AmmunitionType ammoType, XKILL_Enums::FiringModeType firingModeType)
{
	attribute_ptr->ammunition[ammoType] = Ammunition(standardAmmunitions[ammoType]);
	attribute_ptr->ammunition[ammoType].currentTotalNrOfShots = attribute_ptr->ammunition[ammoType].initialTotalNrOfShots;
	attribute_ptr->firingMode[firingModeType] = FiringMode(standardFiringModes[firingModeType]);
}

void MutatorSettings::setupAttribute(Attribute_WeaponStats* attribute)
{
	setupAttribute(attribute, attribute->currentAmmunitionType, attribute->currentFiringModeType);
}

void MutatorSettings::setupAttribute(Attribute_WeaponStats* attribute, XKILL_Enums::AmmunitionType ammoType, XKILL_Enums::FiringModeType firingModeType)
{
	attribute->ammunition[ammoType] = Ammunition(standardAmmunitions[ammoType]);
	attribute->ammunition[ammoType].currentTotalNrOfShots = attribute->ammunition[ammoType].initialTotalNrOfShots;
	attribute->firingMode[firingModeType] = FiringMode(standardFiringModes[firingModeType]);
}

Ammunition& MutatorSettings::getStandardAmmunition(XKILL_Enums::AmmunitionType type)
{
	return standardAmmunitions[type];
}

FiringMode& MutatorSettings::getStandardFiringMode(XKILL_Enums::FiringModeType type)
{
	return standardFiringModes[type];
}

void MutatorSettings::initStandardAmmunition()
{
	Ammunition* ammo;

	ammo = &standardAmmunitions[XKILL_Enums::AmmunitionType::BULLET];

	ammo->isReloading						= false;
	ammo->type								= XKILL_Enums::AmmunitionType::BULLET;
	ammo->damage							= 20.0f;
	ammo->explosionSphereInitialRadius		= 0.0f;
	ammo->explosionSphereFinalRadius		= 0.0f;
	ammo->explosionSphereExplosionDuration	= 0.0f;
	ammo->explosive							= false;
	ammo->nrOfProjectilesPerSalvo			= 1;
	ammo->displacementSphereRadius			= 0.0f;
	ammo->speed								= 60.0f;
	ammo->spreadConeRadius					= 0.0f;
	ammo->initialTotalNrOfShots				= 100;
	ammo->currentTotalNrOfShots				= ammo->initialTotalNrOfShots;
	ammo->maxTotalNrOfShots					= 100;
	ammo->velocityVariation					= 0.0f;
	ammo->canShootWhileReloading			= false;

	ammo = &standardAmmunitions[XKILL_Enums::AmmunitionType::SCATTER];

	ammo->isReloading						= false;
	ammo->type								= XKILL_Enums::AmmunitionType::SCATTER;
	ammo->damage							= 2.0f;
	ammo->explosionSphereInitialRadius		= 0.0f;
	ammo->explosionSphereFinalRadius		= 0.0f;
	ammo->explosionSphereExplosionDuration	= 0.0f;
	ammo->explosive							= false;
	ammo->nrOfProjectilesPerSalvo			= 10;
	ammo->displacementSphereRadius			= 0.02f;
	ammo->speed								= 30.0f;
	ammo->spreadConeRadius					= 0.1f;
	ammo->initialTotalNrOfShots				= 50;
	ammo->currentTotalNrOfShots				= ammo->initialTotalNrOfShots;
	ammo->maxTotalNrOfShots					= 100;
	ammo->velocityVariation					= 0.5f;
	ammo->canShootWhileReloading			= true;

	ammo = &standardAmmunitions[XKILL_Enums::AmmunitionType::EXPLOSIVE];

	ammo->isReloading						= false;
	ammo->type								= XKILL_Enums::AmmunitionType::EXPLOSIVE;
	ammo->damage							= 50.0f;
	ammo->explosionSphereInitialRadius		= 0.0f;
	ammo->explosionSphereFinalRadius		= 3.0f;
	ammo->explosionSphereExplosionDuration	= 1.0f;
	ammo->explosive							= true;
	ammo->nrOfProjectilesPerSalvo			= 1;
	ammo->displacementSphereRadius			= 0.0f;
	ammo->speed								= 10.0f;
	ammo->spreadConeRadius					= 0.0f;
	ammo->initialTotalNrOfShots				= 5;
	ammo->currentTotalNrOfShots				= ammo->initialTotalNrOfShots;
	ammo->maxTotalNrOfShots					= 10;
	ammo->velocityVariation					= 0.0f;
	ammo->canShootWhileReloading			= false;
}

void MutatorSettings::initStandardFiringModes()
{
	FiringMode* firingMode;

	firingMode = &standardFiringModes[XKILL_Enums::FiringModeType::SINGLE];

	firingMode->type					= XKILL_Enums::FiringModeType::SINGLE;
	firingMode->canShootBullet			= true;
	firingMode->canShootExplosive		= true;
	firingMode->canShootScatter			= true;
	firingMode->clipSize				= 1;
	firingMode->damageModifier			= 5.0f;
	firingMode->explosionSphereModifier	= 1.2f;
	firingMode->cooldownBetweenShots	= 1.0f;
	firingMode->reloadTime				= 1.0f;
	firingMode->cooldownLeft			= firingMode->cooldownBetweenShots;
	firingMode->reloadTimeLeft			= 0.0f;

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		firingMode->nrOfShotsLeftInClip[i] = 0;
	}

	firingMode = &standardFiringModes[XKILL_Enums::FiringModeType::SEMI];

	firingMode->type					= XKILL_Enums::FiringModeType::SEMI;
	firingMode->canShootBullet			= true;
	firingMode->canShootExplosive		= true;
	firingMode->canShootScatter			= true;
	firingMode->clipSize				= 10;
	firingMode->damageModifier			= 3.0f;
	firingMode->explosionSphereModifier	= 0.6f;
	firingMode->cooldownBetweenShots	= 0.0f;
	firingMode->reloadTime				= 2.0f;
	firingMode->cooldownLeft			= firingMode->cooldownBetweenShots;
	firingMode->reloadTimeLeft			= 0.0f;

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		firingMode->nrOfShotsLeftInClip[i] = 0;
	}

	firingMode = &standardFiringModes[XKILL_Enums::FiringModeType::AUTO];

	firingMode->type					= XKILL_Enums::FiringModeType::AUTO;
	firingMode->canShootBullet			= true;
	firingMode->canShootExplosive		= true;
	firingMode->canShootScatter			= true;
	firingMode->clipSize				= 20;
	firingMode->damageModifier			= 1.0f;
	firingMode->explosionSphereModifier	= 0.30f;
	firingMode->cooldownBetweenShots	= 0.1f;
	firingMode->reloadTime				= 5.0f;
	firingMode->cooldownLeft			= firingMode->cooldownBetweenShots;
	firingMode->reloadTimeLeft			= 0.0f;

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		firingMode->nrOfShotsLeftInClip[i] = 0;
	}
}