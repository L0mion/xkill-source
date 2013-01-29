#pragma once

#include "dllUtilities.h"

//Needed in "Event_CreatePickupable", "Event_CreatePickupablesSpawnPoint", "Attribute_PickupablesSpawnPoint", "Attribute_Pickupable"
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
	GRAVITY
};