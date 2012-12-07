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
	SUBSCRIBE_TO_EVENT(this, EVENT_ENTITIES_COLLIDING);
	return true;
}

void GameComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_ENTITIES_COLLIDING:
		event_EntitiesColliding(static_cast<Event_EntitiesColliding*>(e));
		break;
	default:
		break;
	}
}

void GameComponent::event_EntitiesColliding(Event_EntitiesColliding* e)
{
	std::cout << "GameComponent::onEvent, EVENT_PROJECTILECOLLIDINGWITHPLAYER" << std::endl;

	// fetch Entities so we can inspect their attributes
	std::vector<Entity>* allEntity; GET_ENTITIES(allEntity);
	Entity* e1 = &allEntity->at(e->e1_index);
	Entity* e2 = &allEntity->at(e->e2_index);

	// check bullet logic
	if(e1->hasAttribute(ATTRIBUTE_PLAYER))
		
	//if(e1->hasAttribute(ATTRIBUTE_PRO))
	// Remove projectile entity
	SEND_EVENT(&Event_RemoveEntity(0));

	//Lower player health
	//AttributeManager::getInstance()->playerAttributes_
	//PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes_.getAllAttributes();

	//projectileCollidingWithPlayer->playerId;
}


void GameComponent::onUpdate(float delta)
{
	// Fetches attributes from AttributeManager through the use of EventManager.
	// This can be used from everywhere EventManager is known. 
	std::vector<int>* playerOwners;					GET_ATTRIBUTE_OWNERS(playerOwners, ATTRIBUTE_PLAYER);
	std::vector<PlayerAttribute>* allPlayer;		GET_ATTRIBUTES(allPlayer, PlayerAttribute, ATTRIBUTE_PLAYER);
	std::vector<InputAttribute>* allInput;			GET_ATTRIBUTES(allInput, InputAttribute, ATTRIBUTE_INPUT);
	std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>* allPosition;	GET_ATTRIBUTES(allPosition, PositionAttribute, ATTRIBUTE_POSITION);

	for(unsigned i=0; i<playerOwners->size(); i++)
	{
		if(playerOwners->at(i)!=0)
		{
			// Fetch attributes
			PlayerAttribute* player		=	&allPlayer->at(i);
			InputAttribute* input		=	&allInput->at(player->inputAttribute.index);
			RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);
			SpatialAttribute* spatial	=	&allSpatial->at(render->spatialAttribute.index);
			PositionAttribute* position	=	&allPosition->at(spatial->positionAttribute.index);

			if(input->fire)
			{
				//float* position = new float[3];
				//position[0] = 0.0f; position[1] = 0.0f; position[2] = 0.0f;
				//float* direction = new float[3];
				//direction[0] = 1.0f; position[1] = 0.0f; position[2] = 0.0f;
				//Event_createProjectile projectile(position, direction);
				//delete position;
				//delete direction;

				Float3 pos;
				pos.x = position->position.x;
				pos.y = position->position.y;
				pos.z = position->position.z;

				Float4 direction;
				direction.x = spatial->rotation.x;
				direction.y = spatial->rotation.y;
				direction.z = spatial->rotation.z;
				direction.w = spatial->rotation.w;
				Event_CreateProjectile projectile(pos, direction);
				SEND_EVENT(&projectile);
				input->fire = false;
			}


			
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
			*/

			/*std::cout 
			<< player->id				<< "\t"
			<< playerOwners->at(i)		<< "\t"
			<< player->name				<< "\t"
			<< position->position.x		<< "\t"
			<< position->position.y		<< "\t"
			<< position->position.z		<< "\t"
			<< std::endl;*/
		}
	}
}
