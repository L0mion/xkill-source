#pragma once

#include "dllUtilities.h"

class Event;

/// Interface needed for receiving Events
/** 
\ingroup UTILITIES
*/

class IObserver
{
public:
	IObserver(){}
	~IObserver(){}
	virtual void onEvent(Event* e){}
	virtual void onUpdate(float delta){}
};

//
// Components info
//

/// Components execute \ref ATTRIBUTES in a orderly fashion.
/** 
Most funcitonallity should be implemented as a Component.

A Component is the workhorse of the game and processes all of its associated
attributes in a orderly fashion. Since every Entity can be added or removed 
from multiple Components even during Runtime we retain the benefits of a
Component-Oriented approach without sacrificing performance.

\defgroup COMPONENTS Components
\ingroup ARCHITECTURE
*/


/// Interface needed as a Component. Used in ComponentManager
/** 
\ingroup UTILITIES
*/

class IComponent
{
public:
	IComponent();
	~IComponent();
	virtual void onUpdate(float delta) = 0;
};