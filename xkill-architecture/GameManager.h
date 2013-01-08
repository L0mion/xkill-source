#pragma once

/// The duct tape allowing all parts of the game to communicate.
/** 
The architecture is heaviliy based around the flexibility of Component-Based
Programming and Event passing, but at the same time tries to embrace a
Data-Oriented philosphy in which each Entity is broken down into multiple 
\ref COMPONENTS, grouped together in memory and processed in batches allowing
for some serious optimizations.

The bread and butter of the architecture is the use of \ref ATTRIBUTES.
An Attribute can be shared among multiple Components allowing
each Component to work with the same data independently with minimal overhead.

A Component is the workhorse of the game and processes all of its associated
\ref ATTRIBUTES in an orderly fashion. Since every Entity can be added or removed 
from multiple Components even during runtime we retain the benefits of a
Component-Oriented approach without sacrificing performance.

\ref COMPONENTS can communicate with other Components through
the use of events (which is part of xkill-utilities). However, 
creating events for everything is cumbersome and hard to maintain 
so every component should strive to work indepentently.

\defgroup ARCHITECTURE XKILL - Architecture
\image html https://dl.dropbox.com/u/12273871/DOXYGEN/Architectur.png
*/

#include "dllArchitecture.h"
//class ComponentManager;
#include "ComponentManager.h"
class EntityManager;

/// The entry point of the architecture
/** 
GameManager is responsible for setting up the initial game state.
It is also responsible for refreshing the ComponentManager which
in turn updates most of the game logic.

It can also be used as an early testing environment for simulating 
the game by sending an Event, creating and deleting an Entity and such.

\ingroup ARCHITECTURE
*/

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

//#include <vld.h>

class DLL_A GameManager
{
private:
	ComponentManager* componentManager_;
	EntityManager* entityManager_;
public:
	GameManager();
	~GameManager();

	bool init(HWND windowHandle, HWND parentWindowHandle);
	void update(float delta);
};