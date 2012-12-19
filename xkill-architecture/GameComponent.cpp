#include "GameComponent.h"
#include "AttributeManager.h"
#include <DirectXMath.h>

#include <iostream>
#include <ctime>

GameComponent::GameComponent(void)
{
	SUBSCRIBE_TO_EVENT(this, EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);	
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);	
}

GameComponent::~GameComponent(void)
{
}

bool GameComponent::init()
{
	// Fetch list of stuff used in logic
	GET_ENTITIES(allEntity);
	GET_ATTRIBUTE_OWNERS(allPhysicsOwner, ATTRIBUTE_PHYSICS);

	GET_ATTRIBUTES(playerAttributes_, PlayerAttribute, ATTRIBUTE_PLAYER);
	GET_ATTRIBUTES(healthAttributes_, HealthAttribute, ATTRIBUTE_HEALTH);
	GET_ATTRIBUTES(cameraAttributes_, CameraAttribute, ATTRIBUTE_CAMERA);
	GET_ATTRIBUTES(inputAttributes_, InputAttribute, ATTRIBUTE_INPUT);
	GET_ATTRIBUTES(renderAttributes_, RenderAttribute, ATTRIBUTE_RENDER);
	GET_ATTRIBUTES(spatialAttributes_, SpatialAttribute, ATTRIBUTE_SPATIAL);
	GET_ATTRIBUTES(positionAttributes_, PositionAttribute, ATTRIBUTE_POSITION);
	GET_ATTRIBUTES(projectileAttributes_, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
	GET_ATTRIBUTES(physicsAttributes_, PhysicsAttribute, ATTRIBUTE_PHYSICS);
	GET_ATTRIBUTES(spawnPointAttributes_, SpawnPointAttribute, ATTRIBUTE_SPAWNPOINT);
	GET_ATTRIBUTES(weaponStatsAttributes_, WeaponStatsAttribute, ATTRIBUTE_WEAPONSTATS);

	SEND_EVENT(&Event_CreateSpawnPoint(Float3(-1.5f, 3.0f, 0.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(1.0f, 5.0f, 0.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(0.0f, 0.0f, -5.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(1.0f, 1.0f, 1.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(0.0f, 0.0f, -5.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(4.0f, 4.0f, 4.0f), 2.0f));

	srand ( time(NULL) );

	return true;
}

void GameComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_PHYSICS_ATTRIBUTES_COLLIDING:
		event_PhysicsAttributesColliding(static_cast<Event_PhysicsAttributesColliding*>(e));
		break;
	case EVENT_START_DEATHMATCH:
		event_StartDeathmatch(static_cast<Event_StartDeathmatch*>(e));
		break;
	case EVENT_END_DEATHMATCH:
		event_EndDeathmatch(static_cast<Event_EndDeathmatch*>(e));
		break;
	default:
		break;
	}
}

void GameComponent::onUpdate(float delta)
{
	//Handles updates of player attributes
	std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
	for(unsigned i=0; i<playerAttributesOwners->size(); i++)
	{
		if(playerAttributesOwners->at(i)!=0)
		{
			//Extract attributes from a playerAttribute
			PlayerAttribute* player		=	&playerAttributes_->at(i);
			HealthAttribute* health		=	&healthAttributes_->at(player->healthAttribute.index);
			CameraAttribute* camera		=	&cameraAttributes_->at(player->cameraAttribute.index);
			InputAttribute* input		=	&inputAttributes_->at(player->inputAttribute.index);
			RenderAttribute* render		=	&renderAttributes_->at(player->renderAttribute.index);
			WeaponStatsAttribute* weaponStats = &weaponStatsAttributes_->at(player->weaponStatsAttribute.index);
			SpatialAttribute* spatial	=	&spatialAttributes_->at(render->spatialAttribute.index); //Extract spatial attribute from the render attribute from the above playerAttribute
			PositionAttribute* position	=	&positionAttributes_->at(spatial->positionAttribute.index); //Extract position attribute from the above spatial attribute
			
			//Deathmatch end logic
			if(player->priority >= 4)
			{
				//SEND_EVENT(&Event_EndDeathmatch());
			}

			if(input->changeWeapon)
			{
				static int weapon = 0;
				input->changeWeapon = false;

				if(weapon == 0)
				{
  					weaponStats->setWeaponToDebugMachineGun();
				}
				else if(weapon == 1)
				{
					weaponStats->setWeaponStats(WeaponStatsAttribute::SCATTER, WeaponStatsAttribute::AUTO);
				}
				weapon++;

				if(weapon == 2)
				{
					weapon = 0;
				}
			}

			//Firing logic
			if(input->fire)
			{
				input->fire = false;

				if(weaponStats->nrOfShotsLeftInClip > 0 && weaponStats->cooldownLeft <= 0.0f)
				{
					if(weaponStats->totalNrOfShots != -1) //Special case: debug machine gun. Unlimited number of shots.
					{
						weaponStats->cooldownLeft = weaponStats->cooldownBetweenShots;
						weaponStats->totalNrOfShots--;
						weaponStats->nrOfShotsLeftInClip--;
					}

					// Position
					Float3 pos;
					pos.x = position->position.x;
					pos.y = position->position.y;
					pos.z = position->position.z;
					
					// extract camera orientation to determine velocity
					DirectX::XMFLOAT3 lookAtXMFloat3;
					lookAtXMFloat3.x = camera->mat_view._13;
					lookAtXMFloat3.y = camera->mat_view._23;
					lookAtXMFloat3.z = camera->mat_view._33;
					DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtXMFloat3);
					lookAt = DirectX::XMVector3Normalize(lookAt);

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

					Float4 rotation = Float4(orientationQuaternionX, orientationQuaternionY, orientationQuaternionZ, orientationQuaternionW);

					DirectX::XMVECTOR newLookAt;
					float randomLO;
					float randomHI;
					// Send "Event_CreateProjectile" for each projectile in a shot. Scatter has more than one projectile per shot.
					for(int j=0;j<weaponStats->nrOfProjectilesForEachShot;j++)
					{
						randomLO = -weaponStats->spreadConeRadius*0.5f;
						randomHI = weaponStats->spreadConeRadius*0.5f;

						lookAtXMFloat3.x = DirectX::XMVectorGetX(lookAt);
						lookAtXMFloat3.y = DirectX::XMVectorGetY(lookAt);
						lookAtXMFloat3.z = DirectX::XMVectorGetZ(lookAt);
						
						//Randomize spread cone values (direction of velocity)
						lookAtXMFloat3.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
						lookAtXMFloat3.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
						lookAtXMFloat3.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
						newLookAt = DirectX::XMLoadFloat3(&lookAtXMFloat3);
						newLookAt = DirectX::XMVector3Normalize(newLookAt);
						lookAtXMFloat3.x = DirectX::XMVectorGetX(newLookAt);
						lookAtXMFloat3.y = DirectX::XMVectorGetY(newLookAt);
						lookAtXMFloat3.z = DirectX::XMVectorGetZ(newLookAt);

						Float3 velocity(lookAtXMFloat3.x, lookAtXMFloat3.y, lookAtXMFloat3.z);
						velocity.x *= weaponStats->velocityOfEachProjectile;
						velocity.y *= weaponStats->velocityOfEachProjectile;
						velocity.z *= weaponStats->velocityOfEachProjectile;

						// add displacement on position (this should be based on the collision shape of the player model)
						float d = 0.5f;
						pos.x += lookAtXMFloat3.x*d;
						pos.y += lookAtXMFloat3.y*d;
						pos.z += lookAtXMFloat3.z*d;

						//Randomize displacement of each projectile preventing all projectiles spawning from
						randomLO = -weaponStats->displacementSphereRadius*0.5f;
						randomHI = weaponStats->displacementSphereRadius*0.5f;
						pos.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
						pos.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
						pos.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
						SEND_EVENT(&Event_CreateProjectile(pos, velocity, rotation, weaponStats->damgeOfEachProjectile, playerAttributesOwners->at(i), weaponStats->isExplosive));
					}
				}
				else if(weaponStats->nrOfShotsLeftInClip <= 0)
				{
					DEBUGPRINT("Cannot shoot: Out of ammo. Currently reloading.");
				}
				else if(weaponStats->cooldownLeft > 0)
				{
					DEBUGPRINT("Cannot shoot: weapon cooldown. Be patient.");
				}
			}

			//Health and respawn logic for players
			if(health->health <= 0) //A player is dead
			{
				//If an appropriate spawnpoint was found: spawn at it; otherwise: spawn at origo.
				SpawnPointAttribute* spawnPointAttribute = findUnoccupiedSpawnPoint();
				if(spawnPointAttribute != nullptr)
				{
					PositionAttribute* spawnPointPositionAttribute;
					spawnPointPositionAttribute = &positionAttributes_->at(spawnPointAttribute->positionAttribute.index);
					position->position = spawnPointPositionAttribute->position; //Set player position attribute
				}
				else
				{
					position->position = Float3(0.0f, 0.0f, 0.0f);
				}
				
				health->health = health->startHealth; // restore player health
			}
		}
	}

	//Handle updates of projectile attributes
	std::vector<int>* projectileAttributesOwners;		GET_ATTRIBUTE_OWNERS(projectileAttributesOwners, ATTRIBUTE_PROJECTILE);
	for(unsigned i=0; i<projectileAttributesOwners->size(); i++)
	{
		if(projectileAttributesOwners->at(i)!=0)
		{
			ProjectileAttribute* projectile = &projectileAttributes_->at(i);
			projectile->currentLifeTimeLeft -= delta;
			if(projectile->currentLifeTimeLeft <= 0)
			{
				DEBUGPRINT("Projectile entity " << projectileAttributesOwners->at(i) << " has no lifetime left");
				
				//Remove projectile entity
				Event_RemoveEntity removeEntityEvent(projectileAttributesOwners->at(i));
				SEND_EVENT(&removeEntityEvent);
			}
		}
	}

	//Handles updates of spawn point attributes (update "timeSinceLastSpawn" timer)
	std::vector<int>* spawnPointAttributesOwners;		GET_ATTRIBUTE_OWNERS(spawnPointAttributesOwners, ATTRIBUTE_SPAWNPOINT);
	for(unsigned i=0; i<spawnPointAttributesOwners->size(); i++)
	{
		if(spawnPointAttributesOwners->at(i)!=0)
		{
			SpawnPointAttribute* spawnPoint = &spawnPointAttributes_->at(i);
			spawnPoint->timeSinceLastSpawn += delta;
		}
	}

	//Handles updates of weapon stats attributes
	std::vector<int>* weaponStatsAttributesOwners;		GET_ATTRIBUTE_OWNERS(weaponStatsAttributesOwners, ATTRIBUTE_WEAPONSTATS);
	for(unsigned i=0; i<weaponStatsAttributesOwners->size(); i++)
	{
		if(weaponStatsAttributesOwners->at(i)!=0)
		{
			WeaponStatsAttribute* weaponStats = &weaponStatsAttributes_->at(i);
			
			//Cooldown after each shot
			weaponStats->cooldownLeft -= delta;

			//Reloading
			if(weaponStats->totalNrOfShots > 0 && weaponStats->nrOfShotsLeftInClip <= 0)
			{
				//DEBUGPRINT("Reloading...");
				weaponStats->reloadTimeLeft -= delta;
				if(weaponStats->reloadTimeLeft <= 0)
				{
					weaponStats->reloadTimeLeft = weaponStats->reloadTime;

					if(weaponStats->clipSize > weaponStats->totalNrOfShots)
					{
						weaponStats->nrOfShotsLeftInClip = weaponStats->totalNrOfShots;
					}
					else
					{
						weaponStats->nrOfShotsLeftInClip = weaponStats->clipSize;
					}
					weaponStats->totalNrOfShots -= weaponStats->nrOfShotsLeftInClip;
				}
			}
		}
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
								std::vector<ProjectileAttribute>* projectileAttributes_;	GET_ATTRIBUTES(projectileAttributes_, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
								std::vector<PlayerAttribute>* allPlayers;			GET_ATTRIBUTES(allPlayers, PlayerAttribute, ATTRIBUTE_PLAYER);
							
								std::vector<int> projectileId = entity2->getAttributes(ATTRIBUTE_PROJECTILE);
								
								for(unsigned i=0;i<projectileId.size();i++)
								{
									int entityIdOfProjectileCreator = projectileAttributes_->at(projectileId.at(i)).entityIdOfCreator;
									Entity* creatorOfProjectilePlayerEntity = &allEntity->at(entityIdOfProjectileCreator);
									std::vector<int> playerId = creatorOfProjectilePlayerEntity->getAttributes(ATTRIBUTE_PLAYER);

									for(unsigned i=0;i<playerId.size();i++)
									{
										PlayerAttribute* player = &allPlayers->at(playerId.at(i));
										player->priority++;
										DEBUGPRINT("Player with entity id " << entityIdOfProjectileCreator << " killed player with entity id " << entity1->getID());
									}
								}
							}

							SEND_EVENT(&Event_PlayerDeath());
						}
						DEBUGPRINT("DAMAGEEVENT Entity " << entity2->getID() << " damage: " <<  damage->damage << " Entity " << entity1->getID() << " health " << health->health);
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

		if(entity2->hasAttribute(ATTRIBUTE_PHYSICS) && !entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
		{
			//Set gravity on projectiles colliding with physics objects
			std::vector<PhysicsAttribute>* allPhysics; GET_ATTRIBUTES(allPhysics, PhysicsAttribute, ATTRIBUTE_PHYSICS);
			std::vector<int> physicsId = entity1->getAttributes(ATTRIBUTE_PHYSICS);
			for(unsigned i=0;i<physicsId.size();i++)
			{
				PhysicsAttribute* physicsAttribute = &allPhysics->at(physicsId.at(i));
				physicsAttribute->gravity = Float3(0.0f, -10.0f, 0.0f);
				physicsAttribute->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
			}

			//Shorten lifetime of projectile colliding with physics objects
			std::vector<ProjectileAttribute>* allProjectile; GET_ATTRIBUTES(allProjectile, ProjectileAttribute, ATTRIBUTE_PROJECTILE);
			std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);
			for(unsigned i=0;i<projectileId.size();i++)
			{
				ProjectileAttribute* projectileAttribute = &allProjectile->at(projectileId.at(i));
				if(projectileAttribute->currentLifeTimeLeft > 0.2f)
				{
					projectileAttribute->currentLifeTimeLeft = 0.15f;
				}

				//Explosion handling. In progress.
				/*
				if(projectileAttribute->explodeOnImnpact)
				{
					projectileAttribute->currentLifeTimeLeft = 0.0f;

					//Extract projectile position.
					PhysicsAttribute* projectilePhysicsAttribute = &physicsAttributes_->at(projectileAttribute->physicsAttribute.index);
					SpatialAttribute* projectileSpatialAttribute = &spatialAttributes_->at(projectilePhysicsAttribute->spatialAttribute.index);
					PositionAttribute* projectilePositionAttribute = &positionAttributes_->at(projectileSpatialAttribute->positionAttribute.index);

					SEND_EVENT(&Event_CreateExplosionSphere(projectilePositionAttribute->position, projectileAttribute->explosionSphereRadius));
				}
				*/
			}
		}
	}
}

void GameComponent::event_EndDeathmatch(Event_EndDeathmatch* e)
{
	DEBUGPRINT("x--------------x");
	DEBUGPRINT("-x------------x-");
	DEBUGPRINT("DEATHMATCH ENDED");
	DEBUGPRINT("-x------------x-");
	DEBUGPRINT("x--------------x");
}



SpawnPointAttribute* GameComponent::findUnoccupiedSpawnPoint()
{
	SpawnPointAttribute* foundSpawnPoint = nullptr;
	std::vector<int>* spawnPointAttributesOwners;	GET_ATTRIBUTE_OWNERS(spawnPointAttributesOwners, ATTRIBUTE_SPAWNPOINT);
	
	//Special cases: *no spawn point, return nullptr.
	int nrOfSpawnPoints = spawnPointAttributesOwners->size();
	if(nrOfSpawnPoints < 1)
	{
		DEBUGPRINT("GameComponent::findUnoccupiedSpawnPoint - No spawn point found.");
		return foundSpawnPoint;
	}

	//Iterate through all spawn points to find all unoccupied spawn points.
	std::vector<SpawnPointAttribute*> unoccupiedSpawnPointAttributes;
	for(unsigned int i=0; i<nrOfSpawnPoints; i++)
	{
		if(spawnPointAttributesOwners->at(i)!=0)
		{
			foundSpawnPoint = &spawnPointAttributes_->at(i);
			PositionAttribute* spawnPointPositionAttribute;
			spawnPointPositionAttribute = &positionAttributes_->at(foundSpawnPoint->positionAttribute.index);
		
			//Iterate through all living players to find if any of them are inside the current spawn point radius.
			bool occupiedSpawnPoint = false;
			std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
			int nrOfPlayers = playerAttributesOwners->size();
			for(unsigned int j=0; j<nrOfPlayers; j++)
			{
				if(playerAttributesOwners->at(j)!=0)
				{
					PlayerAttribute* player	= &playerAttributes_->at(j);
					HealthAttribute* health	= &healthAttributes_->at(player->healthAttribute.index);

					if(health->health > 0) //If the current player is alive.
					{
						//Extract player position.
						RenderAttribute* render = &renderAttributes_->at(player->renderAttribute.index);
						SpatialAttribute* spatial = &spatialAttributes_->at(render->spatialAttribute.index);
						PositionAttribute* playerPosition = &positionAttributes_->at(spatial->positionAttribute.index);

						float x1, x2, z1, z2;
						x1 = spawnPointPositionAttribute->position.x;
						x2 = playerPosition->position.x;
						z1 = spawnPointPositionAttribute->position.z;
						z2 = playerPosition->position.z;

						float distanceBetweenCurrentPlayerAndCurrentSpawnPoint = sqrtf( ((x1-x2)*(x1-x2)) + ((z1-z2)*(z1-z2)) );
						if(distanceBetweenCurrentPlayerAndCurrentSpawnPoint < foundSpawnPoint->spawnArea) //A player is inside the spawn point radius.
						{
							occupiedSpawnPoint = true;
							break;
						}
					}
				}
			}
			if(!occupiedSpawnPoint)
			{
				unoccupiedSpawnPointAttributes.push_back(foundSpawnPoint); //This vector will be iterated below.
			}
		}
	}

	//Iterate through all unoccupied spawn points (found in the above loop) to find the spawn point that least recently spawned a player.
	int nrOfUnoccupiedSpawnPoints = unoccupiedSpawnPointAttributes.size();
	int longestTimeSinceLastSpawnIndex = -1;
	float longestTimeSinceLastSpawn = 0.0f;
	for(unsigned int i=0; i<nrOfUnoccupiedSpawnPoints; i++)
	{
		foundSpawnPoint = unoccupiedSpawnPointAttributes.at(i);
  		if(foundSpawnPoint->timeSinceLastSpawn >= longestTimeSinceLastSpawn)
		{
			longestTimeSinceLastSpawn = foundSpawnPoint->timeSinceLastSpawn;
 			longestTimeSinceLastSpawnIndex = i;
		}
	}
	if(longestTimeSinceLastSpawnIndex > -1)
	{
		foundSpawnPoint = unoccupiedSpawnPointAttributes.at(longestTimeSinceLastSpawnIndex);
	}

	//If all spawnpoints are occupied, pick one at random.
	if(nrOfUnoccupiedSpawnPoints < 1)
	{
		int pseudoRandomSpawnPointAttributeId = rand()%nrOfSpawnPoints;
		foundSpawnPoint = &spawnPointAttributes_->at(pseudoRandomSpawnPointAttributeId);
	}

	//Reset spawn point timer.
	if(foundSpawnPoint != nullptr)
		foundSpawnPoint->timeSinceLastSpawn = 0.0f;

	return foundSpawnPoint;
}

void GameComponent::event_StartDeathmatch( Event_StartDeathmatch* e )
{
	// Delete players
	std::vector<int>* playerAttributesOwners;		GET_ATTRIBUTE_OWNERS(playerAttributesOwners, ATTRIBUTE_PLAYER);
	for(unsigned i=0; i<playerAttributesOwners->size(); i++)
	{
		if(playerAttributesOwners->at(i)!=0)
		{
			//SEND_EVENT(&Event_RemoveEntity(playerAttributesOwners->at(i)));
		}
	}

	// Create new players
	for(unsigned i=0; i<e->num_players; i++)
	{
		SEND_EVENT(&Event_CreateEntity(PLAYER));
	}
	SEND_EVENT(&Event_WindowResize(800,800));
	// Set state to deathmatch
	GET_STATE() =  STATE_DEATHMATCH;
}
