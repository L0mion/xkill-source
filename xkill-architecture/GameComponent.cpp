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
	SUBSCRIBE_TO_EVENT(this, EVENT_PROJECTILECOLLIDINGWITHPLAYER);
	return true;
}

void GameComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	if(type == EVENT_PROJECTILECOLLIDINGWITHPLAYER) 
	{
		std::cout << "GameComponent::onEvent, EVENT_PROJECTILECOLLIDINGWITHPLAYER" << std::endl;

		//Remove projectile entity
		Event_ProjectileCollidingWithPlayer* projectileCollidingWithPlayer = static_cast<Event_ProjectileCollidingWithPlayer*>(e);
		Event_Remove_Entity removeEntityEvent(projectileCollidingWithPlayer->projectileId);
		SEND_EVENT(&removeEntityEvent);

		//Lower player health
		//AttributeManager::getInstance()->playerAttributes_
		//PlayerAttribute* playerAttribute = AttributeManager::getInstance()->playerAttributes_.getAllAttributes();

		projectileCollidingWithPlayer->playerId;
	}
}

void GameComponent::onUpdate(float delta)
{
	// Fetches attributes from AttributeManager through the use of EventManager.
	// This can be used from everywhere EventManager is known.
	std::vector<PlayerAttribute>* allPlayer;		GET_ATTRIBUTES(allPlayer, PlayerAttribute, ATTRIBUTE_PLAYER);
	std::vector<CameraAttribute>* allCameras;		GET_ATTRIBUTES(allCameras, CameraAttribute, ATTRIBUTE_CAMERA);
	std::vector<InputAttribute>* allInput;			GET_ATTRIBUTES(allInput, InputAttribute, ATTRIBUTE_INPUT);
	std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>* allPosition;	GET_ATTRIBUTES(allPosition, PositionAttribute, ATTRIBUTE_POSITION);
	std::vector<CameraAttribute>* allCamera;		GET_ATTRIBUTES(allCamera, CameraAttribute, ATTRIBUTE_CAMERA);

	std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
	for(unsigned i=0; i<playerAttributesOwners->size(); i++)
	{
		if(playerAttributesOwners->at(i)!=0)
		{
			// Fetch attributes
			PlayerAttribute* player		=	&allPlayer->at(i);
			CameraAttribute* camera		=	&allCameras->at(player->cameraAttribute.index);
			InputAttribute* input		=	&allInput->at(player->inputAttribute.index);
			RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);
			SpatialAttribute* spatial	=	&allSpatial->at(render->spatialAttribute.index);
			PositionAttribute* position	=	&allPosition->at(spatial->positionAttribute.index);

			if(input->fire)
			{
				Float3 pos;
				pos.x = position->position.x;
				pos.y = position->position.y;
				pos.z = position->position.z;

				float lookAtX = camera->mat_view._11;
				float lookAtY = camera->mat_view._12;
				float lookAtZ = camera->mat_view._13;
				//float ffff = camera->mat_view._13;
				Float4 direction(lookAtX, lookAtY, lookAtZ, 1.0f);
				
				//direction.x = spatial->rotation.x;
				//direction.y = spatial->rotation.y;
				//direction.z = spatial->rotation.z;
				//direction.w = spatial->rotation.w;
				Event_createProjectile projectile(pos, direction);
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