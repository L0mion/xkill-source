//#pragma once
//
//#include "IObserver.h"
//#include "dllUtilities.h"
//
////
//// Components info
////
//
///// Components execute \ref attributes in a orderly fashion.
///** 
//Most funcitonallity should be implemented as a Component.
//
//A Component is the workhorse of the game and processes all of its associated
//attributes in a orderly fashion. Since every Entity can be added or removed 
//from multiple Components even during Runtime we retain the benefits of a
//Component-Oriented approach without sacrificing performance.
//
//\defgroup components Components
//\ingroup achitecture
//*/
//
//
///// Interface needed as a Component. Used in ComponentManager
///** 
//\ingroup utilities
//*/
//
//class DLL_U IComponent
//{
//public:
//	virtual void onUpdate(float delta) = 0;
//};




//#pragma once
//
//#include "dllUtilities.h"
//
//class Event;

/// Interface needed for receiving Events
/** 
\ingroup utilities
*/

/*
class DLL_U IComponent
{
public:
	virtual void onEvent(Event* e) = 0;
};
*/