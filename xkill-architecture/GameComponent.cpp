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

	//Crate two spawn points
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(0.0f, 1.0f, -5.0f)));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(0.0f, 5.0f, 0.0f)));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(1.0f, 4.0f, 0.0f)));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(2.0f, 3.0f, 0.0f)));

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
						
						//If a player was killed by the collision, give priority (score) to the player that created the projectile
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

					// remove damage entity
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

		std::vector<ProjectileAttribute>* allProjectile; GET_ATTRIBUTES(allProjectile, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
		std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);

		if(entity2->hasAttribute(ATTRIBUTE_PHYSICS) && !entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
		{
			//Shorten lifetime of projectile colliding with physics objects

			for(int i=0;i<projectileId.size();i++)
			{
				ProjectileAttribute* projectileAttribute = &allProjectile->at(projectileId.at(i));
				if(projectileAttribute->currentLifeTimeLeft > 0.4f)
				{
				projectileAttribute->currentLifeTimeLeft = 0.3f;
				}
			}

			//Set gravity on projectiles colliding with physics objects
			std::vector<PhysicsAttribute>* allPhysics; GET_ATTRIBUTES(allPhysics, PhysicsAttribute, ATTRIBUTE_PHYSICS);
			std::vector<int> physicsId = entity1->getAttributes(ATTRIBUTE_PHYSICS);
			for(int i=0;i<physicsId.size();i++)
			{
				PhysicsAttribute* physicsAttribute = &allPhysics->at(physicsId.at(i));
				physicsAttribute->gravity = Float3(0.0f, -10.0f, 0.0f);
				physicsAttribute->linearVelocity = Float3(0.0f, 0.0f, 0.0f); //Does not seem to work
				int a = 2;
			}

			//Shorten lifetime of projectile colliding with physics objects
			std::vector<ProjectileAttribute>* allProjectile; GET_ATTRIBUTES(allProjectile, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
			std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);
			for(int i=0;i<projectileId.size();i++)
			{
				ProjectileAttribute* projectileAttribute = &allProjectile->at(projectileId.at(i));
				if(projectileAttribute->currentLifeTimeLeft > 1.2f)
				{
					projectileAttribute->currentLifeTimeLeft = 1.15f;
				}
			}
		}

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
	
	std::vector<SpawnPointAttribute>* allSpawnPoints;	GET_ATTRIBUTES(allSpawnPoints, SpawnPointAttribute, ATTRIBUTE_SPAWNPOINT);
	std::vector<int>* spawnPointAttributesOwners;		GET_ATTRIBUTE_OWNERS(spawnPointAttributesOwners, ATTRIBUTE_SPAWNPOINT);

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

			if(input->fire) //Create a projectile
			{
				input->fire = false;

				// Position
				Float3 pos;
				pos.x = position->position.x;
				pos.y = position->position.y;
				pos.z = position->position.z;

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
				velocity.x *= 1000.0f;
				velocity.y *= 1000.0f;
				velocity.z *= 1000.0f;
				// add rotation displacement on position 
				float d = 0.2f;
				pos.x += lookAtX*d;
				pos.y += lookAtY*d;
				pos.z += lookAtZ*d;
				
				// Rotation
				DirectX::XMMATRIX rotationMatrix(	camera->mat_view._11,	camera->mat_view._21,	camera->mat_view._31,	0.0f,
													camera->mat_view._12,	camera->mat_view._22,	camera->mat_view._32,	0.0f, 
													camera->mat_view._13,	camera->mat_view._23,	camera->mat_view._33,	0.0f,
													0.0f,					0.0f,					0.0f,					1.0f);

				DirectX::XMVECTOR orientationQuaternion = DirectX::XMQuaternionRotationMatrix(rotationMatrix);
				float orientationQuaternionX = DirectX::XMVectorGetX(orientationQuaternion);
				float orientationQuaternionY = DirectX::XMVectorGetY(orientationQuaternion);
				float orientationQuaternionZ = DirectX::XMVectorGetZ(orientationQuaternion);
				float orientationQuaternionW = DirectX::XMVectorGetW(orientationQuaternion);

				Float4 rot = Float4(orientationQuaternionX, orientationQuaternionY, orientationQuaternionZ, orientationQuaternionW);

				Float3 gravity = Float3(0.0f, 0.0f, 0.0f);

				// Send event
				SEND_EVENT(&Event_CreateProjectile(pos, velocity, rot, gravity, playerAttributesOwners->at(i)));
				input->fire = false;
			}

			//Health and respawn logic for players
			SpawnPointAttribute* spawnPoint;
			int longestTimeSinceLastSpawnIndex = -1;
			float longestTimeSinceLastSpawn = 0.0f;
			if(health->health <= 0) 
			{
				for(unsigned i=0; i<spawnPointAttributesOwners->size(); i++)
				{
					if(spawnPointAttributesOwners->at(i)!=0)
					{
						spawnPoint = &allSpawnPoints->at(i);
  						if(spawnPoint->timeSinceLastSpawn >= longestTimeSinceLastSpawn)
						{
							longestTimeSinceLastSpawn = spawnPoint->timeSinceLastSpawn;
 							longestTimeSinceLastSpawnIndex = i;
						}
					}
 				}

				//Set player position to the spawn point position and reset the spawn point timer
				if(longestTimeSinceLastSpawnIndex > -1)
				{
					spawnPoint = &allSpawnPoints->at(longestTimeSinceLastSpawnIndex);
					PositionAttribute* newPosition	= &allPositions->at(spawnPoint->positionAttribute.index);
					position->position = newPosition->position;
					spawnPoint->timeSinceLastSpawn = 0.0f;
				}
				else //if no spawn point was found
				{
					position->position = Float3();
				}

				// restore health player have health on next life
				health->health = health->startHealth;
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
#ifdef XKILL_DEBUG
				std::cout << "Projectile entity " << projectileAttributesOwners->at(i) << " has no lifetime left" << std::endl;
#endif
				//Remove projectile entity
				Event_RemoveEntity removeEntityEvent(projectileAttributesOwners->at(i));
				SEND_EVENT(&removeEntityEvent);
			}
		}
	}

	//Handle updates of spawn point attributes (update "timeSinceLastSpawn" timer)
	for(unsigned i=0; i<spawnPointAttributesOwners->size(); i++)
	{
		if(spawnPointAttributesOwners->at(i)!=0)
		{
			SpawnPointAttribute* spawnPoint = &allSpawnPoints->at(i);
			spawnPoint->timeSinceLastSpawn += delta;
		}
	}
}