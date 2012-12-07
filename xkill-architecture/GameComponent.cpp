#include "GameComponent.h"
#include "AttributeManager.h"
#include <DirectXMath.h>

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
		event_PhysicsAttributesColliding(static_cast<Event_PhysicsAttributesColliding*>(e));
		break;
	default:
		break;
	}
}

void GameComponent::event_PhysicsAttributesColliding(Event_PhysicsAttributesColliding* e)
{
	std::cout << "GameComponent::onEvent, EVENT_PROJECTILECOLLIDINGWITHPLAYER" << std::endl;

	// Fetch Entities so we can inspect their attributes
	std::vector<Entity>* allEntities; GET_ENTITIES(allEntities);
	Entity* e1 = &allEntities->at(e->e1_index);
	Entity* e2 = &allEntities->at(e->e2_index);

	//Event_Remove_Entity removeEntityEvent(projectileCollidingWithPlayer->projectileEntityId);
	//SEND_EVENT(&removeEntityEvent);
	// check bullet logic

	if(e1->hasAttribute(ATTRIBUTE_PLAYER) ^ e2->hasAttribute(ATTRIBUTE_PROJECTILE))
	{

	}
		
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
	std::vector<PlayerAttribute>* allPlayers;		GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
	std::vector<CameraAttribute>* allCameras;		GET_ATTRIBUTES(allCameras, CameraAttribute, ATTRIBUTE_CAMERA);
	std::vector<InputAttribute>* allInput;			GET_ATTRIBUTES(allInput, InputAttribute, ATTRIBUTE_INPUT);
	std::vector<RenderAttribute>* allRender;		GET_ATTRIBUTES(allRender, RenderAttribute, ATTRIBUTE_RENDER);
	std::vector<SpatialAttribute>* allSpatial;		GET_ATTRIBUTES(allSpatial, SpatialAttribute, ATTRIBUTE_SPATIAL);
	std::vector<PositionAttribute>* allPositions;	GET_ATTRIBUTES(allPositions, PositionAttribute, ATTRIBUTE_POSITION);
	std::vector<ProjectileAttribute>* allProjectiles;	GET_ATTRIBUTES(allProjectiles, ProjectileAttribute, ATTRIBUTE_PROJECTILE);

	//Handle updates of player attributes
	std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
	for(unsigned i=0; i<playerAttributesOwners->size(); i++)
	{
		if(playerAttributesOwners->at(i)!=0)
		{
			// Extract attributes from a playerAttribute
			PlayerAttribute* player		=	&allPlayers->at(i);
			CameraAttribute* camera		=	&allCameras->at(player->cameraAttribute.index);
			InputAttribute* input		=	&allInput->at(player->inputAttribute.index);
			RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);			
			SpatialAttribute* spatial	=	&allSpatial->at(render->spatialAttribute.index); //Extract spatial attribute from the render attribute from the above playerAttribute
			PositionAttribute* position	=	&allPositions->at(spatial->positionAttribute.index); //Extract position attribute from the above spatial attribute

			if(input->fire)
			{
				Float3 pos;
				pos.x = position->position.x;
				pos.y = position->position.y;
				pos.z = position->position.z;

				DirectX::XMFLOAT3 lookAtFloat3;
				lookAtFloat3.x = camera->mat_view._13;
				lookAtFloat3.y = camera->mat_view._12;
				lookAtFloat3.z = camera->mat_view._11;
				DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtFloat3); 
				lookAt = DirectX::XMVector3Normalize(lookAt);
				float x = DirectX::XMVectorGetX(lookAt);
				float y = DirectX::XMVectorGetY(lookAt);
				float z = DirectX::XMVectorGetZ(lookAt);

				Float3 velocity(x, y, z);
				velocity.x *= 1.5f;
				velocity.y *= 1.5f;
				velocity.z *= 1.5f;

				//id of player or id of player entity?
				Event_CreateProjectile projectile(pos, velocity, playerAttributesOwners->at(i));
				SEND_EVENT(&projectile);
				input->fire = false;
			}
		}
	}

	//Handle updates of projectile attributes (lifetime management)
	std::vector<int>* projectileAttributesOwners;		GET_ATTRIBUTE_OWNERS(projectileAttributesOwners, ATTRIBUTE_PROJECTILE);
	for(unsigned i=0; i<projectileAttributesOwners->size(); i++)
	{
		if(projectileAttributesOwners->at(i)!=0)
		{
			ProjectileAttribute* projectile = &allProjectiles->at(i);
			projectile->currentLifeTimeLeft -= delta;

			if(projectile->currentLifeTimeLeft <= 0)
			{
				std::cout << "Projectile entity " << projectileAttributesOwners->at(i) << " has no lifetime left" << std::endl;
				
				//Remove projectile entity
				Event_RemoveEntity removeEntityEvent(projectileAttributesOwners->at(i));
				SEND_EVENT(&removeEntityEvent);
			}
		}
	}
}