#include "GameComponent.h"
#include "CollisionManager.h"
#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/XKILL_Enums.h>
#include <xkill-utilities/MutatorSettings.h>
#include <xkill-utilities/Timer.h>
#include <DirectXMath.h>

#include <ctime>

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

// Iterators
ATTRIBUTES_DECLARE_ALL;


GameComponent::GameComponent(void)
{
	SUBSCRIBE_TO_EVENT(this, EVENT_STARTGAME);
	SUBSCRIBE_TO_EVENT(this, EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_TRANSFER_EVENTS_TO_GAME);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
	SUBSCRIBE_TO_EVENT(this, EVENT_UNLOAD_LEVEL);
}

GameComponent::~GameComponent(void)
{
	for(unsigned int i = 0; i < levelEvents_.size(); i++)
	{
		delete levelEvents_.at(i);
	}
	levelEvents_.clear();
}

bool GameComponent::init()
{
	//Fetch list of stuff used in logic
	GET_ENTITIES(allEntity);

	ATTRIBUTES_INIT_ALL;

	srand((unsigned)time(NULL));
	
	return true;
}

void GameComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_STARTGAME:
		startGame();
		break;
	case EVENT_PHYSICS_ATTRIBUTES_COLLIDING:
		event_PhysicsAttributesColliding(static_cast<Event_PhysicsAttributesColliding*>(e));
		break;
	case EVENT_START_DEATHMATCH:
		event_StartDeathmatch(static_cast<Event_StartDeathmatch*>(e));
		break;
	case EVENT_END_DEATHMATCH:
		event_EndDeathmatch(static_cast<Event_EndDeathmatch*>(e));
		break;
	case EVENT_TRANSFER_EVENTS_TO_GAME:
		event_TransferEventsToGame(static_cast<Event_TransferEventsToGame*>(e));
		break;
	case EVENT_PLAYERDEATH:
		event_PlayerDeath(static_cast<Event_PlayerDeath*>(e));
		break;
	case EVENT_UNLOAD_LEVEL:
		event_UnloadLevel();
		break;
	default:
		break;
	}
}

void GameComponent::onUpdate(float delta)
{
	//--------------------------------------------------------------------------------------
	// Handle player attributes
	//--------------------------------------------------------------------------------------
	while(itrPlayer.hasNext())
	{
		//Fetch player-related attributes
		AttributePtr<Attribute_Player>			ptr_player		=	itrPlayer		.getNext();

		AttributePtr<Attribute_Health>			ptr_health		=	ptr_player	->	ptr_health		;
		AttributePtr<Attribute_Camera>			ptr_camera		=	ptr_player	->	ptr_camera		;
		AttributePtr<Attribute_Input>			ptr_input		=	ptr_player	->	ptr_input		;
		AttributePtr<Attribute_Render>			ptr_render		=	ptr_player	->	ptr_render		;
		AttributePtr<Attribute_WeaponStats>		ptr_weaponStats	=	ptr_player	->	ptr_weaponStats	;
		AttributePtr<Attribute_Spatial>			ptr_spatial		=	ptr_render	->	ptr_spatial		;
		AttributePtr<Attribute_Position>		ptr_position	=	ptr_spatial	->	ptr_position	;
		AttributePtr<Attribute_Physics>			ptr_physics		=	ptr_input	->	ptr_physics		;

		Ammunition* ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
		FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

		//--------------------------------------------------------------------------------------
		// State: player is alive
		//--------------------------------------------------------------------------------------
		if(ptr_health->health > 0.0f && !ptr_player->detectedAsDead)
		{
			//--------------------------------------------------------------------------------------
			// Ammunition logic: Ammunnition change, firing mode change.
			//--------------------------------------------------------------------------------------
			bool ammunitionSwitchReload = false;
			if(ptr_input->changeAmmunitionType)
			{
				ptr_input->changeAmmunitionType = false;
				ptr_weaponStats->currentAmmunitionType = static_cast<XKILL_Enums::AmmunitionType>((ptr_weaponStats->currentAmmunitionType + 1) % XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES);
				bool successfullySwitchedAmmunition = switchAmmunition(ptr_weaponStats);
				if(successfullySwitchedAmmunition)
				{
					ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
				}
				else
				{
					DEBUGPRINT("Failed to switch ammunition. Succeeded in playing 'beep' sound");
					DEBUGPRINT("\a");
				}
			}

			if(ptr_input->changeFiringMode)
			{
				ptr_input->changeFiringMode = false;
				ptr_weaponStats->currentFiringModeType = static_cast<XKILL_Enums::FiringModeType>((ptr_weaponStats->currentFiringModeType + 1) % XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES);
				bool successfullySwitcheFiringModeOrAmmunnition = switchFiringMode(ptr_weaponStats);
				if(successfullySwitcheFiringModeOrAmmunnition)
				{
					firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];
					ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];

					if(firingMode->nrOfShotsLeftInClip[ptr_weaponStats->currentAmmunitionType] > ammo->currentTotalNrOfShots)
					{

					}
				}
				else
				{
					DEBUGPRINT("Failed to switch firing mode. Succeeded in playing 'beep' sound");
					DEBUGPRINT("\a");
				}
			
			}
			int ammoIndex = ammo->type;

			//--------------------------------------------------------------------------------------
			// Non-automatic weapon reload logic (also refer to automatic weapon reload logic)
			//--------------------------------------------------------------------------------------
			if(ptr_input->reload)
			{
				if(firingMode->nrOfShotsLeftInClip[ammoIndex] > 0 && firingMode->nrOfShotsLeftInClip[ammoIndex] != firingMode->clipSize)
				{
					ammo->currentTotalNrOfShots += firingMode->nrOfShotsLeftInClip[ammoIndex];
					firingMode->nrOfShotsLeftInClip[ammoIndex] = 0; //Set nrOfShotsLeftInClip to 0, forcing automatic weapon reload
				}
			}

			//--------------------------------------------------------------------------------------
			// Firing logic: shootProjectile
			//--------------------------------------------------------------------------------------
			if((ptr_input->fire && firingMode->type == XKILL_Enums::FiringModeType::AUTO) || 
				ptr_input->firePressed && (firingMode->type == XKILL_Enums::FiringModeType::SINGLE || firingMode->type == XKILL_Enums::FiringModeType::SEMI))
			{
				ptr_input->fire = false;
				ptr_input->firePressed = false;

				if(firingMode->cooldownBetweenShots >= 0 && firingMode->cooldownLeft <= 0.0f
					&& firingMode->nrOfShotsLeftInClip[ammoIndex] > 0)
				{
					firingMode->cooldownLeft = firingMode->cooldownBetweenShots;
					ammo->currentTotalNrOfShots--;
					firingMode->nrOfShotsLeftInClip[ammoIndex]--;

					shootProjectile(ptr_player->ptr_weaponFireLocation_spatial, ptr_weaponStats);
					SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_FIRE, ptr_position->position, true));
				}
				else if(firingMode->nrOfShotsLeftInClip[ammoIndex] <= 0)
				{
					if(ammo->currentTotalNrOfShots <= 0)
					{
						DEBUGPRINT("Cannot shoot: Out of ammo.");
					}
					else
					{
						DEBUGPRINT("Cannot shoot: Out of ammo in current clip.");
					}
				}
				else if(firingMode->cooldownLeft > 0)
				{
					DEBUGPRINT("Cannot shoot: weapon cooldown. Be patient.");
				}
			}

			//--------------------------------------------------------------------------------------
			// Sprint (run) logic
			//--------------------------------------------------------------------------------------
			if(ptr_input->sprint && ptr_player->canSprint)
			{
				ptr_player->currentSprintTime -= delta;
				if(ptr_player->currentSprintTime < 0)
				{
					ptr_player->canSprint = false;
				}

				ptr_player->currentSpeed = ptr_player->sprintSpeed;
				ptr_input->sprint = false;
			}
			else
			{
				ptr_player->currentSprintTime += delta * ptr_player->sprintRechargeRate;
				if(ptr_player->currentSprintTime > ptr_player->sprintTime)
				{
					ptr_player->currentSprintTime = ptr_player->sprintTime;
					ptr_player->canSprint = true;
				}
				ptr_player->currentSpeed = ptr_player->walkSpeed;
			}

			//--------------------------------------------------------------------------------------
			// Update player aiming ray
			//--------------------------------------------------------------------------------------
			Entity* playerEntity = itrPlayer.owner();
			updateAimingRay(playerEntity, ptr_camera);
			
			//--------------------------------------------------------------------------------------
			// Damage taken bookkeeping (Not tested. Idea was to lower player speed when the player took damage) 
			//-------------------------------------------------------------------------------------
			if(ptr_health->health < ptr_health->healthFromLastFrame)
			{
				ptr_player->timeSinceLastDamageTaken = 0.0f;
			}
			ptr_health->healthFromLastFrame = ptr_health->health;

			ptr_player->timeSinceLastDamageTaken += delta;
			ptr_player->jetpack = false;
		}
		//--------------------------------------------------------------------------------------
		// State: player is neither alive nor dead. Player was alive, but is now detected as being dead.
		//--------------------------------------------------------------------------------------
		else if(ptr_health->health <= 0.0f && !ptr_player->detectedAsDead) //The "detectedAsDead" flag is used to prevent the "Event_PlayerDeath" event from being sent more than once.
		{
			SEND_EVENT(&Event_PlayerDeath(itrPlayer.storageIndex()));
		}
		//--------------------------------------------------------------------------------------
		// State: player is dead
		//--------------------------------------------------------------------------------------
		else if(ptr_player->detectedAsDead) 
		{
			//--------------------------------------------------------------------------------------
			// Handle dead player
			//--------------------------------------------------------------------------------------
			if(!ptr_player->respawnTimer.hasTimerExpired())
			{
				ptr_player->respawnTimer.update(delta);

				float alive = 3.14f/4.0f;
				float dead = 3.14f/3.0f;
				float slerp = (1 - ptr_player->respawnTimer.getTimeLeft()/ptr_player->respawnTimer.getStartTime());
				float fov = slerp*dead + (1-slerp)*alive;
				ptr_player->ptr_camera->fieldOfView = fov;
			}
			//--------------------------------------------------------------------------------------
			// Respawn player
			//--------------------------------------------------------------------------------------
			else 
			{
				//--------------------------------------------------------------------------------------
				// Spawn point
				//--------------------------------------------------------------------------------------
				AttributePtr<Attribute_PlayerSpawnPoint> ptr_spawnPoint = findUnoccupiedSpawnPoint();
				if(ptr_spawnPoint.isValid()) //If an appropriate spawnpoint was found: spawn at it
				{
					AttributePtr<Attribute_Position> ptr_spawnPoint_position = ptr_spawnPoint->ptr_position;
					ptr_position->position = ptr_spawnPoint_position->position; // set player position attribute
					DEBUGPRINT("Player entity " << itrPlayer.ownerId() << " spawned at " << ptr_position->position.x << " " << ptr_position->position.y << " " << ptr_position->position.z << std::endl);
				}
				else //otherwise: spawn at origo.
				{
					ptr_position->position = Float3(0.0f, 0.0f, 0.0f);
					DEBUGPRINT("No spawn point was found. Player entity " << itrPlayer.ownerId() << " spawned at " << ptr_position->position.x << " " << ptr_position->position.y << " " << ptr_position->position.z << std::endl);
				}

				//--------------------------------------------------------------------------------------
				// Reset player
				//--------------------------------------------------------------------------------------
				ptr_physics->gravity = Float3(0.0f, -10.0f, 0.0f);
				ptr_physics->collisionFilterMask = XKILL_Enums::PhysicsAttributeType::EVERYTHING;
				ptr_physics->collisionResponse = true;
				ptr_physics->meshID = ptr_player->meshID_whenAlive;

				ptr_spatial->rotation = Float4(0.0f, 0.0f, 0.0f, 1.0f);
				ptr_camera->up = Float3(0.0f, 1.0f, 0.0f);
				ptr_camera->right = Float3(1.0f, 0.0f, 0.0f);
				ptr_camera->look = Float3(0.0f, 0.0f, 1.0f);
				ptr_physics->reloadDataIntoBulletPhysics = true;
				
				if(ptr_player->corpseEntityId > 0)
				{
					SEND_EVENT(&Event_RemoveEntity(ptr_player->corpseEntityId));
				}

				ptr_player->ptr_render->cull = true;

				ptr_player->corpseEntityId = -1;
				ptr_health->health = ptr_health->maxHealth; // restores player health

				MutatorSettings ms;

				for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
				{
					for(int j = 0; j < XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES; j++)
					{
						ms.setupAttribute(ptr_weaponStats, static_cast<XKILL_Enums::AmmunitionType>(i), static_cast<XKILL_Enums::FiringModeType>(j));
					}
				}

				ptr_player->ptr_camera->fieldOfView =3.14f/4.0f;
				ptr_player->respawnTimer.resetTimer();
				ptr_player->detectedAsDead = false;
				SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_RESPAWN, ptr_position->position, true));
			}
		}

		//--------------------------------------------------------------------------------------
		// Instant respawn of player, used for debugging
		//--------------------------------------------------------------------------------------
		if(ptr_input->killPlayer)
		{
			ptr_health->health = 0.0f;
			ptr_input->killPlayer = false;
			ptr_player->detectedAsDead = true;
			ptr_player->respawnTimer.zeroTimer();
		}
	}

	//--------------------------------------------------------------------------------------
	// Handle projectile attributes
	//--------------------------------------------------------------------------------------
	while(itrProjectile.hasNext())
	{
		AttributePtr<Attribute_Projectile> projectile = itrProjectile.getNext();
		projectile->currentLifeTimeLeft -= delta; //Update projectile lifetime
		if(projectile->currentLifeTimeLeft <= 0)
		{
			DEBUGPRINT("Projectile entity " << itrProjectile.ownerId() << " has no lifetime left");
			SEND_EVENT(&Event_RemoveEntity(itrProjectile.ownerId()));
		}

		/*
		switch(projectile->ammunitionType) //Handle projectile based on ammunitionType
		{
		case XKILL_Enums::AmmunitionType::BULLET:
			break;
		case XKILL_Enums::AmmunitionType::EXPLOSIVE:
			break;
		case XKILL_Enums::AmmunitionType::SCATTER:
			if( ((projectile->totalLifeTime - projectile->currentLifeTimeLeft) < (projectile->totalLifeTime-0.1f)) && projectile->scatterDropped == false)
			{
				//SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GRAVITY, static_cast<void*>(&Float3(0.0f, -10.0f, 0.0f)), projectile->ptr_physics));

				//SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::VELOCITY, static_cast<void*>(&Float3(0.0f, 0.0f, 0.0f)), projectile->ptr_physics));

				//SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::VELOCITYPERCENTAGE, static_cast<void*>(&Float3(0.1f, 0.1f, 0.1f)), projectile->ptr_physics));
				projectile->scatterDropped = true;
			}
			break;
		}
		*/
	}

	//--------------------------------------------------------------------------------------
	// Handle player spawn point attributes
	//--------------------------------------------------------------------------------------
	while(itrPlayerSpawnPoint.hasNext())
	{
		AttributePtr<Attribute_PlayerSpawnPoint> ptr_spawnPoint	= itrPlayerSpawnPoint.getNext();
		ptr_spawnPoint->secondsSinceLastSpawn += delta;
	}

	//--------------------------------------------------------------------------------------
	// Handle pickupables spawn point attributes
	//--------------------------------------------------------------------------------------
	while(itrPickupablesSpawnPoint.hasNext())
	{
		AttributePtr<Attribute_PickupablesSpawnPoint> ptr_pickupablesSpawnPoint = itrPickupablesSpawnPoint.getNext();

		//Timer incrementation
		ptr_pickupablesSpawnPoint->secondsSinceLastPickup += delta;
		ptr_pickupablesSpawnPoint->secondsSinceLastSpawn += delta;

		//Spawn new pickupable, if secondsSinceLastSpawn and secondsSinceLastPickup are enough incremented
		if(ptr_pickupablesSpawnPoint->secondsSinceLastSpawn > ptr_pickupablesSpawnPoint->spawnDelayInSeconds && ptr_pickupablesSpawnPoint->secondsSinceLastPickup > ptr_pickupablesSpawnPoint->spawnDelayInSeconds)
		{
			if(ptr_pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables < ptr_pickupablesSpawnPoint->maxNrOfExistingSpawnedPickupables)
			{
				AttributePtr<Attribute_Position> pickupablesSpawnPointPosition = ptr_pickupablesSpawnPoint->ptr_position;

				int amount;
				switch(ptr_pickupablesSpawnPoint->spawnPickupableType)
				{
				case XKILL_Enums::PickupableType::MEDKIT:
					amount = 20;
					break;
				case XKILL_Enums::PickupableType::AMMUNITION_BULLET:
					amount = 100;
					break;
				case XKILL_Enums::PickupableType::AMMUNITION_SCATTER:
					amount = 50;
					break;
				case XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE:
					amount = 10;
					break;
				case XKILL_Enums::PickupableType::HACK_SPEEDHACK:
					amount = 5000;											//Will be handled as milliseconds
				}

				//Each pickupable knows it pickupablesSpawnPoint creator
				AttributePtr<Attribute_PickupablesSpawnPoint> ptr_creator_pickupablesSpawnPoint = ptr_pickupablesSpawnPoint;
				SEND_EVENT(&Event_CreatePickupable(pickupablesSpawnPointPosition->position, ptr_pickupablesSpawnPoint->spawnPickupableType, ptr_creator_pickupablesSpawnPoint, amount));
				ptr_pickupablesSpawnPoint->secondsSinceLastSpawn = 0.0f;
			}
		}
	}

	//check
	//--------------------------------------------------------------------------------------
	// Handle pickupable attributes
	//--------------------------------------------------------------------------------------
	//while(itrPickupable.hasNext())
	//{
	//	Attribute_Pickupable* pickupable = itrPickupable.getNext();
	//}

	//--------------------------------------------------------------------------------------
	// Handle weapons stats attributes
	//--------------------------------------------------------------------------------------
	while(itrWeaponStats.hasNext())
	{
		AttributePtr<Attribute_WeaponStats> weaponStats = itrWeaponStats.getNext();
		Ammunition* ammo = &weaponStats->ammunition[weaponStats->currentAmmunitionType];
		FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

		int ammoIndex = ammo->type;

		firingMode->cooldownLeft -= delta; // Weapon cooldown logic

		//--------------------------------------------------------------------------------------
		// Automatic weapon reload logic
		//--------------------------------------------------------------------------------------
		if(ammo->currentTotalNrOfShots > 0 && firingMode->nrOfShotsLeftInClip[ammoIndex] <= 0)
		{
			firingMode->reloadTimeLeft -= delta;
			if(firingMode->reloadTimeLeft <= 0)
			{
				firingMode->reloadTimeLeft = firingMode->reloadTime;

				if(firingMode->clipSize > ammo->currentTotalNrOfShots)
				{
					firingMode->nrOfShotsLeftInClip[ammoIndex] = ammo->currentTotalNrOfShots;
				}
				else
				{
					firingMode->nrOfShotsLeftInClip[ammoIndex] = firingMode->clipSize;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	// Handle explosion sphere attributes
	//--------------------------------------------------------------------------------------
	while(itrExplosionSphere.hasNext())
	{
		AttributePtr<Attribute_ExplosionSphere> ptr_explosionSphere = itrExplosionSphere.getNext();
		ptr_explosionSphere->currentLifeTimeLeft -= delta;
		if(ptr_explosionSphere->currentLifeTimeLeft <= 0.0f)
		{
			SEND_EVENT(&Event_RemoveEntity(itrExplosionSphere.ownerId()));
		}
	}
}

void GameComponent::event_PhysicsAttributesColliding(Event_PhysicsAttributesColliding* e)
{
	// Fetch Entities so we can inspect their attributes
	Entity* entity1 = itr_entity->at(itrPhysics.ownerIdAt(e->attribute1_index));
	Entity* entity2 = itr_entity->at(itrPhysics.ownerIdAt(e->attribute2_index));

	// Handle hit reaction on entity 1
	// when colliding with entity 2;

	CollisionManager collisionManager;

	collisionManager.collision_applyDamage(entity1, entity2);
	collisionManager.collision_projectile(entity1, entity2);
	collisionManager.collision_pickupable(entity1, entity2);
	collisionManager.collision_playerVsExplosionSphere(entity1, entity2);
}

void GameComponent::event_EndDeathmatch(Event_EndDeathmatch* e)
{
	// Delete players
	while(itrPlayer.hasNext())
	{
		itrPlayer.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPlayer.ownerId()));
	}

	while(itrPhysics.hasNext())
	{
		itrPhysics.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPhysics.ownerId()));
	}
	while(itrPlayerSpawnPoint.hasNext())
	{
		itrPlayerSpawnPoint.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPlayerSpawnPoint.ownerId()));
	}

	while(itrLightDir.hasNext())
	{
		itrLightDir.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrLightDir.ownerId()));
	}

	while(itrLightPoint.hasNext())
	{
		itrLightPoint.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrLightPoint.ownerId()));
	}

	while(itrLightSpot.hasNext())
	{
		itrLightSpot.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrLightSpot.ownerId()));
	}
}

AttributePtr<Attribute_PlayerSpawnPoint> GameComponent::findUnoccupiedSpawnPoint()
{
	AttributePtr<Attribute_PlayerSpawnPoint> ptr_found_spawnPoint;
	std::vector<AttributePtr<Attribute_PlayerSpawnPoint>> unoccupiedSpawnPoints;

	// Special cases: *no player spawn point, return nullptr.
	int numSpawnPoints = itrPlayerSpawnPoint.storageSize();
	if(numSpawnPoints < 1)
	{
		DEBUGPRINT("GameComponent::findUnoccupiedSpawnPoint - No spawn point found.");
		return ptr_found_spawnPoint;
	}

	//
	// Find all unoccupied player spawn points.
	//

	AttributeIterator<Attribute_PlayerSpawnPoint> itrPlayerSpawnPoint = ATTRIBUTE_MANAGER->playerSpawnPoint.getIterator();
	while(itrPlayerSpawnPoint.hasNext())
	{
		// Fetch attributes
		ptr_found_spawnPoint = itrPlayerSpawnPoint.getNext();
		AttributePtr<Attribute_Position> position_spawnPoint = ptr_found_spawnPoint->ptr_position;

		// To prevent spawncamping, check if player spawnpoint is occupied
		bool isUnoccupied = true;
		AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
		while(itrPlayer.hasNext())
		{
			AttributePtr<Attribute_Player> player		= itrPlayer.getNext();
			AttributePtr<Attribute_Health> ptr_health	= player->ptr_health;

			// If player is alive
			if(ptr_health->health > 0 && !player->detectedAsDead)
			{
				AttributePtr<Attribute_Render>		render	= player->ptr_render;
				AttributePtr<Attribute_Spatial>		spatial	= render->ptr_spatial;
				AttributePtr<Attribute_Position>	position_player	= spatial->ptr_position;

				// calculate distance to player spawn point
				float distanceToSpawnPoint =  position_player->position.distanceTo(position_spawnPoint->position);

				// if a player is within player spawn point radius
				if(distanceToSpawnPoint < ptr_found_spawnPoint->spawnArea)
				{
					// spawn point is occupied, abort further testing
					isUnoccupied = false;
					break;
				}
			}
		}
		// TRUE: Player spawn point is unoccupied, add to vector
		if(isUnoccupied)
		{
			unoccupiedSpawnPoints.push_back(ptr_found_spawnPoint); // this vector will be iterated below.
		}
	}


	// Iterate through all unoccupied player spawn points 
	// (found in the above loop) to find the player spawn 
	// point that least recently spawned a player.

	int nrOfUnoccupiedSpawnPoints = unoccupiedSpawnPoints.size();

	// Find least recently used player spawn point
	int longestTimeSinceLastSpawnIndex = -1;
	float longestTimeSinceLastSpawn = 0.0f;
	for(int i=0; i<nrOfUnoccupiedSpawnPoints; i++)
	{
		ptr_found_spawnPoint = unoccupiedSpawnPoints.at(i);
		if(ptr_found_spawnPoint->secondsSinceLastSpawn >= longestTimeSinceLastSpawn)
		{
			longestTimeSinceLastSpawn = ptr_found_spawnPoint->secondsSinceLastSpawn;
			longestTimeSinceLastSpawnIndex = i;
		}
	}
	if(longestTimeSinceLastSpawnIndex > -1)
	{
		ptr_found_spawnPoint = unoccupiedSpawnPoints.at(longestTimeSinceLastSpawnIndex);
	}

	// If all player spawn points are occupied, pick one at random.
	if(nrOfUnoccupiedSpawnPoints < 1)
	{
		int randomSpawnPointId = Math::randomInt(numSpawnPoints);
		ptr_found_spawnPoint = itrPlayerSpawnPoint.at(randomSpawnPointId);
	}

	// Reset player spawn point timer.
	if(ptr_found_spawnPoint.isValid())
		ptr_found_spawnPoint->secondsSinceLastSpawn = 0.0f;

	return ptr_found_spawnPoint;
}

void GameComponent::event_StartDeathmatch( Event_StartDeathmatch* e )
{
	//while(itrPickupablesSpawnPoint.hasNext())
	//{
	//	itrPickupablesSpawnPoint.getNext();
	//	SEND_EVENT(&Event_RemoveEntity(itrPickupablesSpawnPoint.ownerId()));
	//}

	/*
	while(itrPickupable.hasNext())
	{
	itrPickupable.getNext();
	SEND_EVENT(&Event_RemoveEntity(itrPickupable.ownerId()));
	}
	*/

	//
	// Create level entities
	//

	for(unsigned int i = 0; i < levelEvents_.size(); i++)
	{
		SEND_EVENT(levelEvents_.at(i));
	}

	// Create new players
	for(int i=0; i<e->num_players; i++)
	{
		SEND_EVENT(&Event_CreateEntity(PLAYER));
	}

	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player>			ptr_player		=	itrPlayer		.getNext();
		AttributePtr<Attribute_WeaponStats>		ptr_weaponStats	=	ptr_player	->	ptr_weaponStats	;
		switchFiringMode(ptr_weaponStats);	//Ensure ammunition disablement (selected from menu)
		
		SEND_EVENT(&Event_HackActivated(1000.0f, XKILL_Enums::HackType::JETHACK, ptr_player));
	}

	//Create mesh for debugging fbx-loading.
	SEND_EVENT(&Event_CreateEntity(RENDERABLE));
	// Get window resolution so we can tell renderer to recalculate and resize split screens
	Event_GetWindowResolution event_getWindowResolution;
	SEND_EVENT(&event_getWindowResolution);
	int width = event_getWindowResolution.width;
	int height = event_getWindowResolution.height;
	SEND_EVENT(&Event_WindowResize(width, height));

	//Set state to deathmatch
	GET_STATE() =  STATE_DEATHMATCH;
}

void GameComponent::event_TransferEventsToGame(Event_TransferEventsToGame* e)
{
	levelEvents_ = e->events;
}

void GameComponent::event_PlayerDeath(Event_PlayerDeath* e)
{
	AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(e->playerAttributeIndex);
	AttributePtr<Attribute_Physics> ptr_physics = ptr_player->ptr_input->ptr_physics;
	AttributePtr<Attribute_Health> ptr_health = ptr_player->ptr_health;
	ptr_health->health = 0;

	if(ptr_player->corpseEntityId == -1)
	{
		SEND_EVENT(&Event_CreateCorpse(ptr_player));
		ptr_player->ptr_render->cull = false;
	}

	ptr_physics->angularVelocity = Float3(0.0f, 0.0f, 0.0f);
	ptr_physics->linearVelocity = Float3(0.0f, 0.0f, 0.0f);
	ptr_physics->gravity = Float3(0.0f, -1.0f, 0.0f);
	ptr_physics->collisionFilterMask = XKILL_Enums::PhysicsAttributeType::WORLD;
	ptr_physics->collisionResponse = true;
	ptr_physics->meshID = ptr_player->meshID_whenDead;
	ptr_physics->reloadDataIntoBulletPhysics = true;

	ptr_player->respawnTimer.resetTimer();
	ptr_player->detectedAsDead = true;
}

void GameComponent::event_UnloadLevel()
{
	std::vector<Event*>::iterator it = levelEvents_.begin();
	for(; it != levelEvents_.end(); it++)
	{
		SAFE_DELETE(*it);
	}

	levelEvents_.clear();
}

bool GameComponent::switchAmmunition(AttributePtr<Attribute_WeaponStats> weaponStats)
{
	bool switchedAmmunition = false;
	FiringMode* firingMode = &weaponStats->firingMode[weaponStats->currentFiringModeType];

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		weaponStats->currentAmmunitionType = static_cast<XKILL_Enums::AmmunitionType>((weaponStats->currentAmmunitionType + 1) % XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES);

		//Try switching of ammunition
		if(weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::BULLET && firingMode->canShootBullet || 
			weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::SCATTER && firingMode->canShootScatter || 
			weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE && firingMode->canShootExplosive)
		{
			switchedAmmunition = true;
			break;
		}
	}

	return switchedAmmunition;
}

bool GameComponent::switchFiringMode(AttributePtr<Attribute_WeaponStats> ptr_weaponStats)
{
	bool switchedFiringMode = false;

	FiringMode* firingMode;

	for(int i = 0; i < XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES; i++)
	{
		ptr_weaponStats->currentFiringModeType = static_cast<XKILL_Enums::FiringModeType>((ptr_weaponStats->currentFiringModeType + 1) % XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES);

		firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

		//Try switching of firing mode
		if((ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::BULLET && firingMode->canShootBullet) ||
			(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::SCATTER && firingMode->canShootScatter) ||
			(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE && firingMode->canShootExplosive))
		{
			switchedFiringMode = true;
			break;
		}
		else if(switchAmmunition(ptr_weaponStats)) //switch firing mode AND ammunition
		{
			switchedFiringMode = true;
			break;
		}
	}

	return switchedFiringMode;
}

void GameComponent::updateAimingRay(Entity* playerEntity, AttributePtr<Attribute_Camera> ptr_camera)
{
	std::vector<int> rayAttributeId = playerEntity->getAttributes(ATTRIBUTE_RAY);
	for(unsigned int i=0;i<rayAttributeId.size();i++)
	{
		//--------------------------------------------------------------------------------------
		// Draw ray and hit points by putting it in an attribute for Bullet Physics to handle
		//--------------------------------------------------------------------------------------
		//Float3 lookAtFarPlaneHorizon = ptr_camera->ptr_spatial->rotation.quaternionToVector();
		Float3 lookAtFarPlaneHorizon = ptr_camera->look;
		lookAtFarPlaneHorizon.normalize();
		lookAtFarPlaneHorizon.x = lookAtFarPlaneHorizon.x*ptr_camera->zFar;
		lookAtFarPlaneHorizon.y = lookAtFarPlaneHorizon.y*ptr_camera->zFar;
		lookAtFarPlaneHorizon.z = lookAtFarPlaneHorizon.z*ptr_camera->zFar;

		AttributePtr<Attribute_Ray> ray = itrRay.at(rayAttributeId.at(i));
		ray->from = ptr_camera->ptr_spatial->ptr_position->position;
		//ray->from = ptr_player->ptr_weaponFireLocation_spatial->ptr_position->position;
		ray->to = lookAtFarPlaneHorizon + ray->from;

		//--------------------------------------------------------------------------------------
		// Do ray test directly by sending an "Event_GetEntityIdOfPhysicsObjectHitByRay" event. The result is stored in the event.
		//--------------------------------------------------------------------------------------
		std::vector<int> rayCastingPlayerAttributeId = playerEntity->getAttributes(ATTRIBUTE_PLAYER);
		for(unsigned int i=0;i<rayCastingPlayerAttributeId.size();i++)
		{
			AttributePtr<Attribute_Player> rayCastingPlayerAttribute = itrPlayer.at(rayCastingPlayerAttributeId.at(i));
					
			if(rayCastingPlayerAttribute->executing) //shoot Laser Automatic Sniper Execution Ray (do not look into the beam)
			{
				short collisionFilterMask = XKILL_Enums::PhysicsAttributeType::PLAYER | XKILL_Enums::PhysicsAttributeType::WORLD;
				Event_GetEntityIdOfPhysicsObjectHitByRay ev(ray->from, ray->to, collisionFilterMask);
				SEND_EVENT(&ev);
				
				Entity* entityHitByRay = &allEntity->at(ev.closest_entityId);
				std::vector<int> playerHitByRayAttributeId = entityHitByRay->getAttributes(ATTRIBUTE_PLAYER);
				for(unsigned int j=0;j<playerHitByRayAttributeId.size();j++)
				{
					//Player hit by his own ray
					if(ev.closest_entityId == playerEntity->getID())
					{
						//SHOW_MESSAGEBOX("Player hit by ray casted by himself. The current code assumes that this is unwanted behavior, therefore this message box is now brought to you");
					}
					else if(entityHitByRay->hasAttribute(ATTRIBUTE_PLAYER))
					{
						DEBUGPRINT("Player with attribute id " << playerHitByRayAttributeId.at(j) << "hit by execution laser");
						
						SEND_EVENT(&Event_PlayerDeath(playerHitByRayAttributeId.at(j)));

						std::vector<int> positionID = entityHitByRay->getAttributes(ATTRIBUTE_POSITION);
						for(unsigned int i = 0; i < positionID.size(); i++)
						{
							AttributePtr<Attribute_Position> ptr_position = itrPosition.at(positionID[i]);
							bool use3DAudio = true;
							SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_DEATH, ptr_position->position, use3DAudio));
						}

						rayCastingPlayerAttribute->priority++;
					}
				}
			}
		}
	}
}

void GameComponent::shootProjectile( AttributePtr<Attribute_Spatial> ptr_spatial, AttributePtr<Attribute_WeaponStats> ptr_weaponStats )
{
	Ammunition* ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
	FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

	Float3 pos		=	ptr_spatial->ptr_position->position;
	Float4 rot		=	ptr_spatial->rotation;
	Float3 lookAt	=	ptr_spatial->rotation.quaternionToVector();

	// Send "Event_CreateProjectile" for each projectile in a shot. Scatter has more than one projectile per shot.
	for(unsigned int j = 0; j < ammo->nrOfProjectilesPerSalvo; j++)
	{
		Float3 new_pos = pos;
		Float3 new_lookAt = lookAt;

		// Randomize spreadConeRadius cone values (direction of velocity)
		if(ammo->spreadConeRadius != 0.0f)
		{
			float RANDOM_LOW = -ammo->spreadConeRadius*0.5f;
			float RANDOM_HIGH = ammo->spreadConeRadius*0.5f;

			new_lookAt.x += Math::randomFloat(RANDOM_LOW, RANDOM_HIGH);
			new_lookAt.y += Math::randomFloat(RANDOM_LOW, RANDOM_HIGH);
			new_lookAt.z += Math::randomFloat(RANDOM_LOW, RANDOM_HIGH);

			new_lookAt.normalize();
		}

		// Determine velocity
		Float3 velocity = new_lookAt * ammo->speed;

		// Randomize velocity for each consecutive projectile
		if(ammo->velocityVariation != 0.0f)
		{
			float RANDOM_LOW = 1 - ammo->velocityVariation*0.5f;
			float RANDOM_HIGH = 1 + ammo->velocityVariation*0.5f;

			float velocityDifference = Math::randomFloat(RANDOM_LOW, RANDOM_HIGH);

			velocity.x *= velocityDifference;
			velocity.y *= velocityDifference;
			velocity.z *= velocityDifference;
		}

		// Add displacement on position (this should be based on the collision shape of the player model)
		/*float d = 0.5f;
		new_pos.x += new_lookAt.x*d;
		new_pos.y += new_lookAt.y*d;
		new_pos.z += new_lookAt.z*d;*/

		//// randomize displacement of each projectile preventing them from spawning at the same position
		//if(ammo->displacementSphereRadius != 0.0f)
		//{
		//	randomLO = -ammo->displacementSphereRadius *0.5f;
		//	randomHI = ammo->displacementSphereRadius *0.5f;
		//	scatterPos.x += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
		//	scatterPos.y += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
		//	scatterPos.z += randomLO + (float)rand()/((float)RAND_MAX/(randomHI-randomLO));
		//}

		SEND_EVENT(&Event_CreateProjectile(new_pos, velocity, rot, itrPlayer.ownerId(), ammo->type, firingMode->type, ammo->damage));
	}
}

void GameComponent::startGame()
{
	// Hide mouse & menu so it is not distracting from game play
	SEND_EVENT(&Event_SetMouseLock(true));
	SEND_EVENT(&Event_EnableHud(true));
	SEND_EVENT(&Event_EnableMenu(false));

	// Make sure game ends properly before starting a new game
	SEND_EVENT(&Event_EndDeathmatch());

	// Set state deathmatch
	GET_STATE() = STATE_DEATHMATCH;

	// Start deathmatch; the only gamemode so far
	// we also have to specify the number of players top start with
	int numPlayers = SETTINGS->numPlayers;
	SEND_EVENT(&Event_StartDeathmatch(numPlayers));
}
