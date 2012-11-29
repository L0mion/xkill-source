#include "GameComponent.h"
#include "AttributeManager.h"

#include <iostream>

GameComponent::GameComponent(void)
{
}

GameComponent::~GameComponent(void)
{
}

bool GameComponent::init()
{
	return true;
}

void GameComponent::onEvent(Event* e)
{
}

void GameComponent::onUpdate(float delta)
{
	std::vector<PlayerAttribute>* attributes = AttributeManager::getInstance()->playerAttributes.getAllAttributes();
	std::vector<int>* owners = AttributeManager::getInstance()->playerAttributes.getAllOwners();

	// Write test
	std::cout << "PLAYERCOMPONENT: Write test" << std::endl;
	for(int i=0; i<(int)(*attributes).size(); i++)
	{
		if(owners->at(i)!=0)
		{
			PlayerAttribute* playerAttribute = &attributes->at(i);
			RenderAttribute* renderAttribute = ATTRIBUTE_CAST(RenderAttribute, renderAttribute, playerAttribute); //Extract render attribute from player attribute
			SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute,spatialAttribute, renderAttribute);
			PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute,positionAttribute, spatialAttribute);
			
			// The player moves
			positionAttribute->position[0]++;

			std::cout 
				<< owners->at(i)			<< "\t"
				<< playerAttribute->name	<< "\t"
				<< positionAttribute->position[0]				<< "\t"
				<< positionAttribute->position[1]				<< "\t"
				<< positionAttribute->position[2]				<< "\t"
				<< std::endl;
		}
	}
}