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
	std::vector<PlayerAttribute>* attributesOfPlayer = AttributeManager::getInstance()->playerAttributes.getAllAttributes();
	std::vector<int>* attributesOfPlayerOwners = AttributeManager::getInstance()->playerAttributes.getAllOwners();

	for(int i=0; i<static_cast<int>(attributesOfPlayer->size()); i++)
	{
		if(attributesOfPlayerOwners->at(i)!=0)
		{
			PlayerAttribute* playerAttribute = &attributesOfPlayer->at(i);
			RenderAttribute* renderAttribute = ATTRIBUTE_CAST(RenderAttribute, renderAttribute, playerAttribute); //Extract render attribute from player attribute
			SpatialAttribute* spatialAttribute = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, renderAttribute);
			PositionAttribute* positionAttribute = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatialAttribute);
			

			//AttributeManager::getInstance()->cameraAttributes.getAllAttributes()->at(playerAttribute->cameraId).spatialAttribute = spatialAttribute;
			//AttributeManager::getInstance()->cameraAttributes.getAllAttributes()->at(playerAttribute->cameraId).spatialAttribute

			//playerAttribute->playerID

			// The player moves
			positionAttribute->position[0];

			std::cout 
				<< attributesOfPlayerOwners->at(i)			<< "\t"
				<< playerAttribute->name	<< "\t"
				<< positionAttribute->position[0]				<< "\t"
				<< positionAttribute->position[1]				<< "\t"
				<< positionAttribute->position[2]				<< "\t"
				<< std::endl;
		}
	}
}