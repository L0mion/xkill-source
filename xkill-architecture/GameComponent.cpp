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

	// Fetch list of stuff used in logic
	GET_ENTITIES(allEntity);
	GET_ATTRIBUTE_OWNERS(allPhysicsOwner, ATTRIBUTE_PHYSICS);

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
	// Fetch Entities so we can inspect their attributes
	Entity* entity1 = &allEntity->at(allPhysicsOwner->at(e->attribute1_index)); //entityOfCollidingAttribute
	Entity* entity2 = &allEntity->at(allPhysicsOwner->at(e->attribute2_index));
	
	//
	// Handle hit reaction on Entity 1 (e1) 
	// when colliding with Entity 2 (e2)
	//

	// health
	if(entity1->hasAttribute(ATTRIBUTE_HEALTH))
	{
		//
		// colliding with...
		//

		// damage
		if(entity2->hasAttribute(ATTRIBUTE_DAMAGE))
		{
			std::vector<DamageAttribute>* allDamage; GET_ATTRIBUTES(allDamage, DamageAttribute, ATTRIBUTE_DAMAGE);
			std::vector<int> damageId = entity2->getAttributes(ATTRIBUTE_DAMAGE);
			for(unsigned i=0; i<damageId.size(); i++)
			{
				DamageAttribute* damage = &allDamage->at(damageId[i]);

				// avoid damage to self
				if(entity1->getID() != damage->owner_entityID)
				{
					// TODO: Damage Player
					std::cout << "COLLISIONEVENT: Entity " << entity1->getID() << " damages Entity " << entity2->getID() << std::endl;

					// TODO: Reward owner of Projectile

				}
			}
		}
	}

	// projectile
	else if(entity1->hasAttribute(ATTRIBUTE_PROJECTILE))
	{
		//
		// colliding with...
		//

		// destroy projectile on impact with everything except the owner who created the projectile
		std::vector<ProjectileAttribute>* allProjectile; GET_ATTRIBUTES(allProjectile, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
		std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);
		for(unsigned i=0; i<projectileId.size(); i++)
		{
			ProjectileAttribute* projectile = &allProjectile->at(projectileId[i]);

			// ignore collision with owner
			if(projectile->entityIdOfCreator != entity2->getID())
			{
				// Destroy the Entity containing the ProjectileAttribute
				std::cout << "COLLISIONEVENT: Projectile " << entity1->getID() << " collides with Entity " << entity2->getID() << std::endl;
				SEND_EVENT(&Event_RemoveEntity(entity1->getID()));
			}
		}
	}
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
	std::vector<HealthAttribute>* allHealth;		GET_ATTRIBUTES(allHealth, HealthAttribute, ATTRIBUTE_HEALTH);

	//Handle updates of player attributes
	std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
	for(unsigned i=0; i<playerAttributesOwners->size(); i++)
	{
		if(playerAttributesOwners->at(i)!=0)
		{
			//Extract attributes from a playerAttribute
			PlayerAttribute* player		=	&allPlayers->at(i);
			CameraAttribute* camera		=	&allCameras->at(player->cameraAttribute.index);
			InputAttribute* input		=	&allInput->at(player->inputAttribute.index);
			RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);
			HealthAttribute* health		=	&allHealth->at(player->healthAttribute.index);
			SpatialAttribute* spatial	=	&allSpatial->at(render->spatialAttribute.index); //Extract spatial attribute from the render attribute from the above playerAttribute
			PositionAttribute* position	=	&allPositions->at(spatial->positionAttribute.index); //Extract position attribute from the above spatial attribute

			if(health->health <= 0)
			{

			}

			if(input->fire)
			{
				// Position
				Float3 pos;
				pos.x = position->position.x;
				pos.y = position->position.y;
				pos.z = position->position.z;

				// Rotation
				Float4 rot = spatial->rotation;
				// extract camera rotation to determine velocity
				DirectX::XMFLOAT3 lookAtFloat3;
				lookAtFloat3.x = camera->mat_view._13;
				lookAtFloat3.y = camera->mat_view._12;
				lookAtFloat3.z = camera->mat_view._11;
				DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtFloat3);
				lookAt = DirectX::XMVector3Normalize(lookAt);
				float x = DirectX::XMVectorGetX(lookAt);
				float y = DirectX::XMVectorGetY(lookAt);
				float z = DirectX::XMVectorGetZ(lookAt);
				// velocity
				Float3 velocity(x, y, z);
				velocity.x *= 750.5f;
				velocity.y *= 750.5f;
				velocity.z *= 750.5f;
				// add rotation displacement on position 
				float d = 1.0f;
				pos.x += x*d;
				pos.y += y*d;
				pos.z += z*d;

				// Velocity
				

				//id of player or id of player entity?
				Event_CreateProjectile projectile(pos, velocity, rot, playerAttributesOwners->at(i));
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