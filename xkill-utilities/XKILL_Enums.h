#pragma once

#include "dllUtilities.h"

namespace XKILL_Enums
{
	enum DLL_U PickupableType
	{
		MEDKIT,
		AMMUNITION_BULLET,
		AMMUNITION_SCATTER,
		AMMUNITION_EXPLOSIVE,
		HACK_SPEEDHACK,
		HACK_JETHACK,
	
		NROFPICKUPABLETYPES
	};

	enum DLL_U ModifyPhysicsObjectData
	{
		GRAVITY,
		VELOCITY,
		VELOCITYPERCENTAGE,
		FLAG_STATIC,
		GIVE_IMPULSE
	};

	enum DLL_U FiringModeType
	{
		SINGLE,
		SEMI,
		AUTO,

		NROFFIRINGMODETYPES
	};
	enum DLL_U AmmunitionType
	{
		BULLET,
		SCATTER,
		EXPLOSIVE,

		NROFAMMUNITIONTYPES
	};

	enum DLL_U HackType
	{
		SPEEDHACK,
		JETHACK,

		NROFHACKTYPES
	};
}