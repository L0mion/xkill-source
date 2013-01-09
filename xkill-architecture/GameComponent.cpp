#include "GameComponent.h"
#include <xkill-utilities/AttributeManager.h>
#include <DirectXMath.h>

#include <iostream>
#include <ctime>

// Iterators
ATTRIBUTES_DECLARE_ALL;


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

	SEND_EVENT(&Event_CreateSpawnPoint(Float3(-1.5f, 3.0f, 0.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(1.0f, 5.0f, 0.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(0.0f, 0.5f, -5.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(1.0f, 1.0f, 1.0f), 2.0f));
	SEND_EVENT(&Event_CreateSpawnPoint(Float3(4.0f, 4.0f, 4.0f), 2.0f));

	ATTRIBUTES_INIT_ALL;

	srand ((unsigned)time(NULL) );

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
	//
	// Update players
	//

	while(itrPlayer.hasNext())
	{
		// Fetch attributes through iterators
		Attribute_Player*		player		=	itrPlayer		.getNext();

		Attribute_Health*		health		=	itrHealth		.at(player->ptr_health);
		Attribute_Camera*		camera		=	itrCamera		.at(player->ptr_camera);
		Attribute_Input*		input		=	itrInput		.at(player->ptr_input);
		Attribute_Render*		render		=	itrRender		.at(player->ptr_render);
		Attribute_WeaponStats*	weaponStats	=	itrWeaponStats	.at(player->ptr_weaponStats);
		Attribute_Spatial*		spatial		=	itrSpatial		.at(render->ptr_spatial);
		Attribute_Position*		position	=	itrPosition		.at(spatial->ptr_position);


		Entity* playerEntity = itrPlayer.owner();
		Attribute_DebugShape* debugShap = itrDebugShape.createAttribute(playerEntity);
		debugShap->ptr_spatial = itrDebugShape.attributePointer(debugShap);


		//
		// End of deathmatch logic
		//

		if(player->priority >= 4)
		{
			//SEND_EVENT(&Event_EndDeathmatch());
		}


		//
		// Ammunition logic
		//

		if(input->changeAmmunitionType)
		{
			input->changeAmmunitionType = false;
			weaponStats->setWeaponStats(static_cast<Attribute_WeaponStats::AmmunitionType>((weaponStats->ammunitionType + 1) % Attribute_WeaponStats::NROFAMUNITIONTYPES), weaponStats->firingMode);
			DEBUGPRINT(std::endl);
			DEBUGPRINT("Ammunition type: " << weaponStats->getAmmunitionTypeAsString());
			DEBUGPRINT("Firing mode: " << weaponStats->getFiringModeAsString());
		}

		if(input->changeFiringMode)
		{
			input->changeFiringMode = false;
			weaponStats->setWeaponStats(weaponStats->ammunitionType, static_cast<Attribute_WeaponStats::FiringMode>((weaponStats->firingMode + 1) % Attribute_WeaponStats::NROFFIRINGMODES));
			DEBUGPRINT(std::endl);
			DEBUGPRINT("Ammunition type: " << weaponStats->getAmmunitionTypeAsString());
			DEBUGPRINT("Firing mode: " << weaponStats->getFiringModeAsString());
		}


		//
		// Firing logic
		//

		if(input->fire)
		{
			input->fire = false;

			if(weaponStats->nrOfShotsLeftInClip > 0 && weaponStats->cooldownLeft <= 0.0f)
			{
				if(weaponStats->totalNrOfShots != -1) // special case: debug machine gun. Unlimited number of shots.
				{
					weaponStats->cooldownLeft = weaponStats->cooldownBetweenShots;
					weaponStats->totalNrOfShots--;
					weaponStats->nrOfShotsLeftInClip--;
				}

				// Position
				Float3 pos = position->position;

				// extract camera orientation to determine velocity
				DirectX::XMFLOAT3 lookAtXMFloat3((float*)&camera->mat_view.getLookAt());

				DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&lookAtXMFloat3);
				lookAt = DirectX::XMVector3Normalize(lookAt);

				// Rotation
				camera->mat_view.getRotationOnly();
				//DirectX::XMMATRIX rotationMatrix((float*)&camera->mat_view);
				DirectX::XMMATRIX rotationMatrix(
					camera->mat_view._11,	camera->mat_view._21,	camera->mat_view._31,	0.0f,
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

					// randomize spread cone values (direction of velocity)
					lookAtXMFloat3.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
					lookAtXMFloat3.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
					lookAtXMFloat3.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
					newLookAt = DirectX::XMLoadFloat3(&lookAtXMFloat3);
					newLookAt = DirectX::XMVector3Normalize(newLookAt);
					lookAtXMFloat3.x = DirectX::XMVectorGetX(newLookAt);
					lookAtXMFloat3.y = DirectX::XMVectorGetY(newLookAt);
					lookAtXMFloat3.z = DirectX::XMVectorGetZ(newLookAt);

					Float3 velocity(lookAtXMFloat3.x, lookAtXMFloat3.y, lookAtXMFloat3.z);
					velocity = velocity * weaponStats->velocityOfEachProjectile;

					// add displacement on position (this should be based on the collision shape of the player model)
					float d = 0.5f;
					pos.x += lookAtXMFloat3.x*d;
					pos.y += lookAtXMFloat3.y*d;
					pos.z += lookAtXMFloat3.z*d;

					// randomize displacement of each projectile preventing all projectiles spawning from
					randomLO = -weaponStats->displacementSphereRadius*0.5f;
					randomHI = weaponStats->displacementSphereRadius*0.5f;
					pos.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
					pos.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
					pos.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));

					SEND_EVENT(&Event_CreateProjectile(pos, velocity, rotation, weaponStats->damgeOfEachProjectile, itrPlayer.ownerId(), weaponStats->isExplosive));
				}
			}
			else if(weaponStats->nrOfShotsLeftInClip <= 0)
			{
				if(weaponStats->totalNrOfShots <= 0)
				{
					DEBUGPRINT("Cannot shoot: Out of ammo.");
				}
				else
				{
					DEBUGPRINT("Cannot shoot: Out of ammo in current clip.");
				}
			}
			else if(weaponStats->cooldownLeft > 0)
			{
				DEBUGPRINT("Cannot shoot: weapon cooldown. Be patient.");
			}
		}

		//
		// Health and respawn logic
		//

		// TRUE: Player is dead
		if(health->health <= 0) 
		{
			// if an appropriate spawnpoint was found: spawn at it; otherwise: spawn at origo.
			Attribute_SpawnPoint* spawnPointAttribute = findUnoccupiedSpawnPoint();
			if(spawnPointAttribute != nullptr)
			{
				Attribute_Position* spawnPointPositionAttribute = itrPosition.at(spawnPointAttribute->ptr_position);
				position->position = spawnPointPositionAttribute->position; // set player position attribute

				DEBUGPRINT("Player entity " << itrPlayer.ownerId() << " spawned at " << position->position.x << " " << position->position.y << " " << position->position.z << std::endl);
			}
			else
			{
				position->position = Float3(0.0f, 0.0f, 0.0f);
			}

			health->health = health->startHealth; // restore player health
			SEND_EVENT(&Event_PlaySound(1));
		}
	}


	//
	// Update projectiles
	//

	while(itrProjectile.hasNext())
	{
		// Fetch attributes
		Attribute_Projectile* projectile = itrProjectile.getNext();

		//
		// Update projectile lifetime
		//

		projectile->currentLifeTimeLeft -= delta;
		if(projectile->currentLifeTimeLeft <= 0)
		{
			DEBUGPRINT("Projectile entity " << itrProjectile.ownerId() << " has no lifetime left");

			// remove projectile entity
			SEND_EVENT(&Event_RemoveEntity(itrProjectile.ownerId()));
		}
	}


	//
	// Update spawn points
	//

	while(itrSpawnPoint.hasNext())
	{
		// spawn delay logic
		Attribute_SpawnPoint* spawnPoint	= itrSpawnPoint.getNext();
		spawnPoint->timeSinceLastSpawn += delta;
	}


	//
	// Update weapons stats
	//

	while(itrWeaponStats.hasNext())
	{
		// Fetch attribute
		Attribute_WeaponStats* weaponStats = itrWeaponStats.getNext();

		//
		// Weapon cooldown logic
		//

		weaponStats->cooldownLeft -= delta;


		//
		// Weapon reload logic
		//

		if(weaponStats->totalNrOfShots > 0 && weaponStats->nrOfShotsLeftInClip <= 0)
		{
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

				DEBUGPRINT("Weapon was automatically reloaded.");
				DEBUGPRINT("Ammo in current clip: " << weaponStats->nrOfShotsLeftInClip);
				DEBUGPRINT("Total number of shots left: " << weaponStats->totalNrOfShots);
			}
		}

	}


	//
	// Update explosion sphere
	//

	while(itrExplosionSphere.hasNext())
	{
		// Fetch attributes
		Attribute_ExplosionSphere* explosionSphere = itrExplosionSphere.getNext();


		//
		// Lifetime logic
		//

		explosionSphere->currentLifeTimeLeft -= delta;
		if(explosionSphere->currentLifeTimeLeft <= 0.0f)
		{
			SEND_EVENT(&Event_RemoveEntity(itrExplosionSphere.ownerId()));
		}
	}
}

void GameComponent::event_PhysicsAttributesColliding(Event_PhysicsAttributesColliding* e)
{
	// Fetch Entities so we can inspect their attributes
	Entity* entity1 = &allEntity->at(itrPhysics.ownerIdAt(e->attribute1_index));
	Entity* entity2 = &allEntity->at(itrPhysics.ownerIdAt(e->attribute2_index));
	

	// Handle hit reaction on entity 1
	// when colliding with entity 2

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
			std::vector<Attribute_Damage>* allDamage = GET_ATTRIBUTES(damage);
			std::vector<int> damageId = entity2->getAttributes(ATTRIBUTE_DAMAGE);

			// fetch health from entity 1
			std::vector<Attribute_Health>* allHealth =  GET_ATTRIBUTES(health);
			std::vector<int> healthId = entity1->getAttributes(ATTRIBUTE_HEALTH);

			for(unsigned i=0; i<damageId.size(); i++)
			{
				Attribute_Damage* damage = &allDamage->at(damageId[i]);

				// avoid damage to self
				if(entity1->getID() != damage->owner_entityID || entity2->hasAttribute(ATTRIBUTE_EXPLOSIONSPHERE))
				{
					// Apply damage to all Health attributes
					for(unsigned i=0; i<healthId.size(); i++)
					{
						Attribute_Health* health = &allHealth->at(healthId[i]);
						health->health -= damage->damage;
						
						// If a player was killed by the collision, give priority (score) to the player that created the DamageAttribute
						if(health->health <= 0)
						{
							std::vector<Attribute_Player>* allPlayers = GET_ATTRIBUTES(player);
							Entity* creatorOfProjectilePlayerEntity = &allEntity->at(damage->owner_entityID);
							std::vector<int> playerId = creatorOfProjectilePlayerEntity->getAttributes(ATTRIBUTE_PLAYER);

							for(unsigned i=0;i<playerId.size();i++)
							{
								Attribute_Player* player = &allPlayers->at(playerId.at(i));
								
								//Award player
								if(entity1->getID() != damage->owner_entityID)
								{
									player->priority++;
								}
								else //Punish player for blowing himself up
								{
									player->priority--;
								}
								
								DEBUGPRINT("Player with entity id " << damage->owner_entityID << " killed player with entity id " << entity1->getID());
							}

							SEND_EVENT(&Event_PlayerDeath());
						}
						else
						{
							SEND_EVENT(&Event_PlaySound(0));
						}
						DEBUGPRINT("DAMAGEEVENT Entity " << entity2->getID() << " damage: " <<  damage->damage << " Entity " << entity1->getID() << " health " << health->health);
					}

					if(entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
					{
						SEND_EVENT(&Event_RemoveEntity(entity2->getID()));
					}
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

		if(entity2->hasAttribute(ATTRIBUTE_PHYSICS) && !entity2->hasAttribute(ATTRIBUTE_PROJECTILE) && !entity2->hasAttribute(ATTRIBUTE_EXPLOSIONSPHERE))
		//if(entity2->hasAttribute(ATTRIBUTE_PHYSICS) && !entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
		{
			//Set gravity on projectiles colliding with physics objects
			std::vector<Attribute_Physics>* allPhysics = GET_ATTRIBUTES(physics);
			std::vector<int> physicsId = entity1->getAttributes(ATTRIBUTE_PHYSICS);
			for(unsigned i=0;i<physicsId.size();i++)
			{
				Attribute_Physics* physicsAttribute = &allPhysics->at(physicsId.at(i));
				physicsAttribute->gravity = Float3(0.0f, -10.0f, 0.0f);
				//TEST
				//physicsAttribute->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
			}

			//Handle PhysicsAttribute of a projectile colliding with another PhysicsAttribute
			std::vector<Attribute_Projectile>* allProjectile = GET_ATTRIBUTES(projectile);
			std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);
			for(unsigned i=0;i<projectileId.size();i++)
			{
				Attribute_Projectile* projectileAttribute = &allProjectile->at(projectileId.at(i));

				//Shorten lifetime of projectile colliding with physics objects
				if(projectileAttribute->currentLifeTimeLeft > 0.2f)
				{
					projectileAttribute->currentLifeTimeLeft = 0.15f;
				}

				//Explosion handling.
				if(projectileAttribute->explodeOnImnpact)
				{
					//Get damage from projectile.
 					std::vector<Attribute_Damage>* allDamage = GET_ATTRIBUTES(damage);
					Attribute_Damage* projectileDamageAttribute;
					if(entity1->hasAttribute(ATTRIBUTE_DAMAGE))
					{
						std::vector<int> damageId = entity1->getAttributes(ATTRIBUTE_DAMAGE);
						for(unsigned i=0;i<damageId.size();i++)
						{
							projectileDamageAttribute = &allDamage->at(damageId.at(i));
						}
					}

 					projectileAttribute->currentLifeTimeLeft = 0.0f;

					//Extract projectile position.
			
					Attribute_Physics* projectilePhysicsAttribute = itrPhysics.at(projectileAttribute->ptr_physics);
					Attribute_Spatial* projectileSpatialAttribute = itrSpatial.at(projectilePhysicsAttribute->ptr_spatial);
					Attribute_Position* projectilePositionAttribute = itrPosition.at(projectileSpatialAttribute->ptr_position);

					//Creates an explosion sphere. Init information is taken from the impacting projectile.
					SEND_EVENT(&Event_CreateExplosionSphere(projectilePositionAttribute->position, projectileAttribute->explosionSphereRadius, projectileDamageAttribute->damage, projectileAttribute->entityIdOfCreator));
				}
			}
			//SEND_EVENT(&Event_RemoveEntity(entity1->getID())); //Crashes somtimes if removed here
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

Attribute_SpawnPoint* GameComponent::findUnoccupiedSpawnPoint()
{
	Attribute_SpawnPoint* foundSpawnPoint = nullptr;
	std::vector<Attribute_SpawnPoint*> unoccupiedSpawnPoints;
	
	// Special cases: *no spawn point, return nullptr.
	int numSpawnPoints = itrSpawnPoint.size();
	if(numSpawnPoints < 1)
	{
		DEBUGPRINT("GameComponent::findUnoccupiedSpawnPoint - No spawn point found.");
		return foundSpawnPoint;
	}

	//
	// Find all unoccupied spawn points.
	//

	AttributeIterator<Attribute_SpawnPoint> itrSpawnPoint = ATTRIBUTE_MANAGER->spawnPoint.getIterator();
	while(itrSpawnPoint.hasNext())
	{
		// Fetch attributes
		foundSpawnPoint = itrSpawnPoint.getNext();
		Attribute_Position* position_spawnPoint = itrPosition.at(foundSpawnPoint->ptr_position);
		
		// To prevent spawncamping, check if spawnpoint is occupied
		bool isUnoccupied = true;
		AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
		while(itrPlayer.hasNext())
		{
			Attribute_Player* player	= itrPlayer.getNext();
			Attribute_Health* health	= itrHealth.at(player->ptr_health);

			// If player is alive
			if(health->health > 0)
			{
				Attribute_Render*	render	= itrRender.at(player->ptr_render);
				Attribute_Spatial*	spatial	= itrSpatial.at(render->ptr_spatial);
				Attribute_Position*	position_player	= itrPosition.at(spatial->ptr_position);

				// calculate distance to spawn point
				float distanceToSpawnPoint =  position_player->position.distanceTo(position_spawnPoint->position);

				// if a player is within spawn point radius
				if(distanceToSpawnPoint < foundSpawnPoint->spawnArea)
				{
					// spawn point is occupied, abort further testing
					isUnoccupied = false;
					break;
				}
			}
		}
		// TRUE: Spawn point is unoccupied, add to vector
		if(isUnoccupied)
		{
			unoccupiedSpawnPoints.push_back(foundSpawnPoint); // this vector will be iterated below.
		}
	}


	// Iterate through all unoccupied spawn points 
	// (found in the above loop) to find the spawn 
	// point that least recently spawned a player.

	int nrOfUnoccupiedSpawnPoints = unoccupiedSpawnPoints.size();
	
	// Find least recently used spawn point
	int longestTimeSinceLastSpawnIndex = -1;
	float longestTimeSinceLastSpawn = 0.0f;
	for(int i=0; i<nrOfUnoccupiedSpawnPoints; i++)
	{
		foundSpawnPoint = unoccupiedSpawnPoints.at(i);
  		if(foundSpawnPoint->timeSinceLastSpawn >= longestTimeSinceLastSpawn)
		{
			longestTimeSinceLastSpawn = foundSpawnPoint->timeSinceLastSpawn;
 			longestTimeSinceLastSpawnIndex = i;
		}
	}
	if(longestTimeSinceLastSpawnIndex > -1)
	{
		foundSpawnPoint = unoccupiedSpawnPoints.at(longestTimeSinceLastSpawnIndex);
	}

	// If all spawn points are occupied, pick one at random.
	if(nrOfUnoccupiedSpawnPoints < 1)
	{
		int randomSpawnPointId = Math::randomInt(numSpawnPoints);
		foundSpawnPoint = itrSpawnPoint.at(randomSpawnPointId);
	}

	// Reset spawn point timer.
	if(foundSpawnPoint != nullptr)
		foundSpawnPoint->timeSinceLastSpawn = 0.0f;

	return foundSpawnPoint;
}

void GameComponent::event_StartDeathmatch( Event_StartDeathmatch* e )
{ 
	// Delete players
	std::vector<int>* playerAttributesOwners = GET_ATTRIBUTE_OWNERS(player);
	for(unsigned i=0; i<playerAttributesOwners->size(); i++)
	{
		if(playerAttributesOwners->at(i)!=0)
		{
			SEND_EVENT(&Event_RemoveEntity(playerAttributesOwners->at(i)));
		}
	}

	// Create new players
	for(int i=0; i<e->num_players; i++)
	{
		SEND_EVENT(&Event_CreateEntity(PLAYER));
	}

	// Get window resolution so we can tell renderer to recalculate and resize split screens
	Event_GetWindowResolution event_getWindowResolution;
	SEND_EVENT(&event_getWindowResolution);
	int width = event_getWindowResolution.width;
	int height = event_getWindowResolution.height;
	SEND_EVENT(&Event_WindowResize(width,height));

	// Set state to deathmatch
	GET_STATE() =  STATE_DEATHMATCH;
}
