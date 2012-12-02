#include "GameComponent.h"
#include "AttributeManager.h"

#include <iostream>

GameComponent::GameComponent(void)
{
	//SHOW_MESSAGEBOX("Game has started");
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
	// Fetches attributes from AttributeManager through the use of EventManager.
	// This can be used from everywhere EventManager is known. 
	std::vector<int>* playerOwners;					GET_ATTRIBUTE_OWNERS(playerOwners, ATT_PLAYER);
	std::vector<PlayerAttribute>* allPlayer;		GET_ATTRIBUTES(allPlayer, PlayerAttribute, ATT_PLAYER);
	std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATT_RENDER);
	std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATT_SPATIAL);
	std::vector<PositionAttribute>* allPosition;	GET_ATTRIBUTES(allPosition, PositionAttribute, ATT_POSITION);

	for(unsigned i=0; i<playerOwners->size(); i++)
	{
		if(playerOwners->at(i)!=0)
		{
			// Fetch attributes
			PlayerAttribute* player		=	&allPlayer		->	at(i);
			RenderAttribute* render		=	&allRender		->	at(player->renderAttribute.index);
			SpatialAttribute* spatial	=	&allSpatial		->	at(render->spatialAttribute.index);
			PositionAttribute* position	=	&allPosition	->	at(spatial->positionAttribute.index);
			
			
			//
			// Proceed with calculation as normal
			//

			/*
			if(player->id == 0) //Handle player 1 (test)
			{
				position->position[0] = position->position[0]+1.0f;
			}
			else if(player->id == 1) //Handle player 2 (test)
			{
				position->position[2] = position->position[2]+1.0f; 
			}
			/*

			/*std::cout 
				<< player->id				<< "\t"
				<< playerOwners->at(i)		<< "\t"
				<< player->name				<< "\t"
				<< position->position[0]	<< "\t"
				<< position->position[1]	<< "\t"
				<< position->position[2]	<< "\t"
				<< std::endl;*/
		}
	}
}