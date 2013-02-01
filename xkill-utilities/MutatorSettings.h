#pragma once
#include "XKILL_Enums.h"
#include "dllUtilities.h"

struct Attribute_WeaponStats;

#include "WeaponStructs.h"

//! Used to store standard values of ammunitions and firing modes and setup weaponstats attributes with these values

class DLL_U MutatorSettings
{
public:
	MutatorSettings();
	~MutatorSettings();

	//! Initializes the current ammunition and firing mode of the attribute
	void setupAttribute(Attribute_WeaponStats* attribute);
	//! Initializes the specified ammunition and firing mode of the attribute
	void setupAttribute(Attribute_WeaponStats* attribute, XKILL_Enums::AmmunitionType ammoType, XKILL_Enums::FiringModeType firingModeType);
	//! Returns the stored standard ammunition of specified type
	Ammunition& getStandardAmmunition(XKILL_Enums::AmmunitionType type);
	//! Returns the stored standard firing mode of specified type
	FiringMode& getStandardFiringMode(XKILL_Enums::FiringModeType type);

private:
	//! Stores a version of each standard ammunition
	static Ammunition standardAmmunitions[XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES];
	//! Stores a version of each standard firing mode
	static FiringMode standardFiringModes[XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES];

	//! Used to make sure that the class is only initialized once, is a dirty workaround
	static bool hasInitialized; 

	//! Initializes the standardAmmunitions array
	void initStandardAmmunition();
	//! Initializes the standardFiringModes array
	void initStandardFiringModes();
};