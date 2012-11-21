#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"


class GameManager
{
	ComponentManager componentManager;
	EntityManager entityManager;
public:
	GameManager()
	{
	}

	void run()
	{
		//
		// Setup Game
		//

		for(int i=0; i<2; i++)
		{
			entityManager.createEntity();
		};


		//
		// Run game
		//

		cout << endl << "Run 1" << endl;
		Event_A e_A;
		EventManager::getInstance()->sendEvent(&e_A);
		componentManager.update(1.0f);
		cin.ignore();

		cout << endl << "Run 2" << endl;
		entityManager.removeEntity(0);
		entityManager.createEntity();
		entityManager.createEntity();
		Event_B e_B;
		EventManager::getInstance()->sendEvent(&e_B);
		componentManager.update(1.0f);
		cin.ignore();

		cout << endl << "Run 3" << endl;
		entityManager.removeEntity(1);
		componentManager.update(1.0f);
		cin.ignore();


		//
		// End game
		//

		// Clean up behind ourselves like good little programmers
	}
};