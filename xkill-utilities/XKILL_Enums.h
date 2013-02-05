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
	
		NROFPICKUPABLETYPES
	};

	enum DLL_U ModifyPhysicsObjectData
	{
		GRAVITY,
		VELOCTIY
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
}