#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "dllArchitecture.h"

class DLL GameManager
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


		//
		// End game
		//

		// Clean up behind ourselves like good little programmers
	}
};