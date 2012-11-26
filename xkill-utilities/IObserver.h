#pragma once

#include "dllUtilities.h"

class Event;

/// Interface needed for receiving Events
/** 
\ingroup utilities
*/

class DLL_U IObserver
{
public:
	virtual void onEvent(Event* e) = 0;
};

class DLL_U IObserver2
{
public:
	virtual void onUpdate(float delta) = 0;
};