#pragma once

#include "dllUtilities.h"

class Event;

class DLL IObserver
{
public:
	virtual void onEvent(Event* e) = 0;
};