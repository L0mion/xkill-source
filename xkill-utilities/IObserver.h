#pragma once

#include "dllUtilities.h"

class Event;

/// Interface needed for receiving Events
/** 
\ingroup utilities
*/

class IObserver
{
public:
	IObserver(){}
	~IObserver(){}
	virtual void onEvent(Event* e){}
	virtual void onUpdate(float delta){}
};

//class DLL_U IComponent
//{
//public:
//	IComponent();
//	~IComponent();
//	virtual void onUpdate(float delta) = 0;
//};