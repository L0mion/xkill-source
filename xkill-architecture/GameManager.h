#pragma once

/// The duct tape that allow all parts of the game to communicate.
/** 
The achitecture is heaviliy based around the flexibility of Component-Based
Programming and Event passing, but at the same time tries to embrace a
Data-Oriented philosphy in which each Entity is broken down into multiple 
Components, grouped together in memory and processed in batches which allows
for some serious optimizations.

The bread and butter of the architecture is the use of attributes.
An Attribute is basically just a Struct which holds Data, however 
each Attribute can be shared among multiple Components allowing
each Component to work with the same data independently of eachother
with minimal overhead.

A Component is the workhorse of the game and processes all of its associated
attributes in an orderly fashion. Since every Entity can be added or removed 
from multiple Components even during Runtime we retain the benefits of a
Component-Oriented approach without sacrificing performance.

If needed Components can communicate with other Components through
the use of events (which is part of xkill-utilities). However, 
creating events for everything is cumbersome and hard to maintain 
so every component should strive to work indepentently from eachother.

\defgroup ARCHITECTURE XKILL - Achitecture
\image html https://dl.dropbox.com/u/12273871/DOXYGEN/Architectur.png
*/

#include "ComponentManager.h"
#include "EntityManager.h"
#include "dllArchitecture.h"

/// The entry point of the architecture
/** 
GameManager is responsible for setting up the initial game state.
It is also responsible for refreshing the ComponentManager which
in turn updates most of the game logic.

It can also be used as an early testing environment for simulating 
the game by sending an Event, creating and deleting an Entity and such.

\ingroup ARCHITECTURE
*/

class DLL_A GameManager
{
	ComponentManager* componentManager;
	EntityManager* entityManager;
public:
	GameManager()
	{
		componentManager = new ComponentManager();
		entityManager = new EntityManager();
	}
	~GameManager()
	{
		delete componentManager;
		delete entityManager;
	}

	void run()
	{
		//
		// Setup Game
		//

		for(int i=0; i<2; i++)
		{
			entityManager->createEntity();
		};


		//
		// Run game
		//

		std::cout << std::endl << "Run 1" << std::endl;
		Event_A e_A;
		EventManager::getInstance()->sendEvent(&e_A);
		componentManager->update(1.0f);
		std::cin.ignore();

		std::cout << std::endl << "Run 2" << std::endl;
		entityManager->removeEntity(0);
		entityManager->createEntity();
		entityManager->createEntity();
		Event_B e_B;
		EventManager::getInstance()->sendEvent(&e_B);
		componentManager->update(1.0f);
		std::cin.ignore();

		std::cout << std::endl << "Run 3" << std::endl;
		entityManager->removeEntity(1);
		componentManager->update(1.0f);
		std::cin.ignore();

		Event_PlaySound playSound(0);

		while(1)
		{
			EventManager::getInstance()->sendEvent(&playSound);
			componentManager->update(1.0f);
		}
		std::cin.ignore();


		//
		// End game
		//

		// Clean up behind ourselves like good little programmers
	}
};