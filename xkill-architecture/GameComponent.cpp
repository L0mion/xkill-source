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
	// Handle hit reaction on entity 1
	// when colliding with entity 2
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
			// fetch damage from entity 2
			std::vector<DamageAttribute>* allDamage; GET_ATTRIBUTES(allDamage, DamageAttribute, ATTRIBUTE_DAMAGE);
			std::vector<int> damageId = entity2->getAttributes(ATTRIBUTE_DAMAGE);

			// fetch health from entity 1
			std::vector<HealthAttribute>* allHealth; GET_ATTRIBUTES(allHealth, HealthAttribute, ATTRIBUTE_HEALTH);
			std::vector<int> healthId = entity1->getAttributes(ATTRIBUTE_HEALTH);

			for(unsigned i=0; i<damageId.size(); i++)
			{
				DamageAttribute* damage = &allDamage->at(damageId[i]);

				// avoid damage to self
				if(entity1->getID() != damage->owner_entityID)
				{
					// Apply damage to all Health attributes
					for(unsigned i=0; i<healthId.size(); i++)
					{
						HealthAttribute* health = &allHealth->at(healthId[i]);
						health->health -= damage->damage;
						
						//If a player was killed by the collision, give priority to the player that created the projectile
						if(health->health <= 0)
						{
							if(entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
							{
								std::vector<ProjectileAttribute>* allProjectiles;	GET_ATTRIBUTES(allProjectiles, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
								std::vector<PlayerAttribute>* allPlayers;			GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
							
								std::vector<int> projectileId = entity2->getAttributes(ATTRIBUTE_PROJECTILE);
								
								for(int i=0;i<projectileId.size();i++)
								{
									int entityIdOfProjectileCreator = allProjectiles->at(projectileId.at(i)).entityIdOfCreator;
									Entity* creatorOfProjectilePlayerEntity = &allEntity->at(entityIdOfProjectileCreator);
									std::vector<int> playerId = creatorOfProjectilePlayerEntity->getAttributes(ATTRIBUTE_PLAYER);

									for(int i=0;i<playerId.size();i++)
									{
										PlayerAttribute* player = &allPlayers->at(playerId.at(i));
										player->priority++;
										std::cout << "Player with entity id " << entityIdOfProjectileCreator << " killed player with entity id " << entity1->getID() << std::endl;
									}
								}
							}

							SEND_EVENT(&Event_PlayerDeath());
						}
						std::cout << "DAMAGEEVENT Entity " << entity2->getID() << " damage: " <<  damage->damage << " Entity " << entity1->getID() << " health " << health->health << std::endl;
					}

					//allPlayers->
					// TODO: Reward owner of Projectile



					// remove projectile
					SEND_EVENT(&Event_RemoveEntity(entity2->getID()));
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

	//	// destroy projectile on impact with everything except the owner who created the projectile
	//	std::vector<ProjectileAttribute>* allProjectile; GET_ATTRIBUTES(allProjectile, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
	//	std::vector<int> projectileId = e1->getAttributes(ATTRIBUTE_PROJECTILE);
	//	for(unsigned i=0; i<projectileId.size(); i++)
	//	{
	//		ProjectileAttribute* projectile = &allProjectile->at(projectileId[i]);

	//		// ignore collision with owner
	//		if(projectile->entityIdOfCreator != e2->getID())
	//		{
	//			// Destroy the Entity containing the ProjectileAttribute
	//			std::cout << "COLLISIONEVENT: Projectile " << e1->getID() << " collides with Entity " << e2->getID() << std::endl;
	//			SEND_EVENT(&Event_RemoveEntity(e1->getID()));
	//		}
	//	}
	//	
	}
}


void GameComponent::onUpdate(float delta)
{
	// Fetches attributes from AttributeManager through the use of EventManager.
	// This can be used from everywhere EventManager is known.
	std::vector<PlayerAttribute>* allPlayers;		GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
	std::vector<HealthAttribute>* allHealth;		GET_ATTRIBUTES(allHealth, HealthAttribute, ATTRIBUTE_HEALTH);
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
			//Extract attributes from a playerAttribute
			PlayerAttribute* player		=	&allPlayers->at(i);
			HealthAttribute* health		=	&allHealth->at(player->healthAttribute.index);
			CameraAttribute* camera		=	&allCameras->at(player->cameraAttribute.index);
			InputAttribute* input		=	&allInput->at(player->inputAttribute.index);
			RenderAttribute* render		=	&allRender->at(player->renderAttribute.index);
			SpatialAttribute* spatial	=	&allSpatial->at(render->spatialAttribute.index); //Extract spatial attribute from the render attribute from the above playerAttribute
			PositionAttribute* position	=	&allPositions->at(spatial->positionAttribute.index); //Extract position attribute from the above spatial attribute

			if(input->fire)
			{
				// Position
				Float3 pos;
				pos.x = position->position.x;
				pos.y = position->position.y;
				pos.z = position->position.z;

				// Rotation

				//TODO: Camera rotation.
				//TODO: velocity direction fix.

				
				// extract camera orientation to determine velocity
				DirectX::XMFLOAT3 lookAtFloat3;
				lookAtFloat3.x = camera->mat_view._13;
				lookAtFloat3.y = camera->mat_view._23;
				lookAtFloat3.z = camera->mat_view._33;
				DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtFloat3);
				lookAt = DirectX::XMVector3Normalize(lookAt);

				//Direction and speed
				float lookAtX = DirectX::XMVectorGetX(lookAt);
				float lookAtY = DirectX::XMVectorGetY(lookAt);
				float lookAtZ = DirectX::XMVectorGetZ(lookAt);
				Float3 velocity(lookAtX, lookAtY, lookAtZ);
				velocity.x *= 750.0f;
				velocity.y *= 750.0f;
				velocity.z *= 750.0f;
				// add rotation displacement on position 
				float d = 1.0f;
				pos.x += lookAtX*d;
				pos.y += lookAtY*d;
				pos.z += lookAtZ*d;
				
				//Retrieve the orientation from the camera look at vector. The projectile will have this orientation.
				//DirectX::XMVECTOR orientationQuaternionAsVectorFromLookAt = DirectX::XMQuaternionRotationRollPitchYawFromVector(lookAt);
				//float orientationQuaternionAsVectorFromLookAtX = DirectX::XMVectorGetX(orientationQuaternionAsVectorFromLookAt);
				//float orientationQuaternionAsVectorFromLookAtY = DirectX::XMVectorGetY(orientationQuaternionAsVectorFromLookAt);
				//float orientationQuaternionAsVectorFromLookAtZ = DirectX::XMVectorGetZ(orientationQuaternionAsVectorFromLookAt);
				//float orientationQuaternionAsVectorFromLookAtW = DirectX::XMVectorGetW(orientationQuaternionAsVectorFromLookAt);
				//Float4 rot = Float4(orientationQuaternionAsVectorFromLookAtX, orientationQuaternionAsVectorFromLookAtY, orientationQuaternionAsVectorFromLookAtZ, orientationQuaternionAsVectorFromLookAtW);

				Float4 rot = spatial->rotation;

				Event_CreateProjectile projectile(pos, velocity, rot, playerAttributesOwners->at(i));
				SEND_EVENT(&projectile);
				input->fire = false;
			}

			// Health logic for players
			if(health->health <= 0)
			{


				// TODO: Respawn entity
				position->position = Float3();

				// restore health player have health on next life
				health->health = 10;
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