#pragma once

#include "dllUtilities.h"

class Event;

/// Interface needed for receiving Events
/** 
\ingroup utilities
*/

class DLL IObserver
{
public:
	virtual void onEvent(Event* e) = 0;
};