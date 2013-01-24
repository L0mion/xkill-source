#pragma once

#include "dllUtilities.h"

struct Attribute_WeaponStats;

#include "WeaponStructs.h"

class DLL_U MutatorSettings
{
public:
	MutatorSettings();
	~MutatorSettings();

	void setupAttribute(Attribute_WeaponStats* attribute);
	void setupAttribute(Attribute_WeaponStats* attribute, Ammunition::AmmunitionType ammoType, FiringMode::FiringModeType firingModeType);
	Ammunition& getStandardAmmunition(Ammunition::AmmunitionType type);
	FiringMode& getStandardFiringMode(FiringMode::FiringModeType type);

private:
	static Ammunition standardAmmunitions[Ammunition::NROFAMUNITIONTYPES];
	static FiringMode standardFiringModes[FiringMode::NROFFIRINGMODETYPES];

	static bool hasInitialized; //Should only initialize once, dirty workaround

	void initStandardAmmunition();
	void initStandardFiringModes();
};