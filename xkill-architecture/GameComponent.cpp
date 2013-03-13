#include "GameComponent.h"
#include "CollisionManager.h"
#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/XKILL_Enums.h>
#include <xkill-utilities/MutatorSettings.h>
#include <xkill-utilities/Timer.h>
#include <xkill-utilities/Converter.h>
#include <DirectXMath.h>

#include <ctime>

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

// Iterators
ATTRIBUTES_DECLARE_ALL;

GameComponent::GameComponent(void)
{
	SUBSCRIBE_TO_EVENT(this, EVENT_STARTGAME);
	SUBSCRIBE_TO_EVENT(this, EVENT_ENDGAME);
	SUBSCRIBE_TO_EVENT(this, EVENT_PHYSICS_ATTRIBUTES_COLLIDING);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_TRANSFER_EVENTS_TO_GAME);
	SUBSCRIBE_TO_EVENT(this, EVENT_PLAYERDEATH);
	SUBSCRIBE_TO_EVENT(this, EVENT_UNLOAD_LEVEL);
	SUBSCRIBE_TO_EVENT(this, EVENT_NULL_PROCESS_STARTED_EXECUTING);
	SUBSCRIBE_TO_EVENT(this, EVENT_NULL_PROCESS_STOPPED_EXECUTING);
}

GameComponent::~GameComponent(void)
{
	for(unsigned int i = 0; i < levelEvents_.size(); i++)
	{
		delete levelEvents_.at(i);
	}
	levelEvents_.clear();

	delete CollisionManager::Instance();
}

bool GameComponent::init()
{
	//Fetch list of stuff used in logic
	GET_ENTITIES(allEntity);
	ATTRIBUTES_INIT_ALL;

	srand((unsigned)time(NULL));

	nullProcessExecuting = false;
	
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
	case EVENT_ENDGAME:
		endGame();
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
	case EVENT_NULL_PROCESS_STARTED_EXECUTING:
		nullProcessExecuting = true;
		break;
	case EVENT_NULL_PROCESS_STOPPED_EXECUTING:
		nullProcessExecuting = false;

		//check lunch
		while(itrPickupable.hasNext())
		{
			AttributePtr<Attribute_Pickupable> ptr_pickupable = itrPickupable.getNext();
			CollisionManager::Instance()->removePickupable(ptr_pickupable);
		}
		break;
	default:
		break;
	}
}
#include <xkill-utilities/Converter.h>
void GameComponent::onUpdate(float delta)
{
	//--------------------------------------------------------------------------------------
	// Handle player attributes
	//--------------------------------------------------------------------------------------
	updatePlayerAttributes(delta);
	
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

				//Each pickupable knows it pickupablesSpawnPoint creator
				int amount = -1; //Deprecated as of 2013-03-07 15.25 (refer to Attribute_Pickupable)
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
		if(ammo->currentTotalNrOfShots > 0 && firingMode->nrOfShotsLeftInClip[ammoIndex] <= 0 && !ammo->isReloading)
		{
			ammo->isReloading = true;
			int nrOfShotsToLoad = firingMode->clipSize;
			if(firingMode->clipSize > ammo->currentTotalNrOfShots)
			{
				nrOfShotsToLoad = ammo->currentTotalNrOfShots;
			}

			float reloadTimeFraction = (1.0f - (static_cast<float>(firingMode->nrOfShotsLeftInClip[ammoIndex])/static_cast<float>(nrOfShotsToLoad)));
			firingMode->reloadTimeLeft = reloadTimeFraction * firingMode->reloadTime;

			if(ammo->canShootWhileReloading)
			{
				firingMode->reloadTimeLeft += firingMode->reloadTime * 0.1f;
			}
		}

		if(ammo->isReloading)
		{
			firingMode->reloadTimeLeft -= delta;

			if(firingMode->reloadTimeLeft <= 0)
			{
				firingMode->reloadTimeLeft = firingMode->reloadTime;
				ammo->isReloading = false;

				if(firingMode->clipSize > ammo->currentTotalNrOfShots)
				{
					firingMode->nrOfShotsLeftInClip[ammoIndex] = ammo->currentTotalNrOfShots;
				}
				else
				{
					firingMode->nrOfShotsLeftInClip[ammoIndex] = firingMode->clipSize;
				}
			}
			else if(ammo->canShootWhileReloading)
			{
				float reloadTimeFraction	= (1.0f - (firingMode->reloadTimeLeft/firingMode->reloadTime));
				float clipFraction			= (static_cast<float>(firingMode->nrOfShotsLeftInClip[ammoIndex])/static_cast<float>(firingMode->clipSize));

				int nrOfShotsLeftToLoad = firingMode->clipSize;
				if(firingMode->clipSize > ammo->currentTotalNrOfShots)
				{
					nrOfShotsLeftToLoad = ammo->currentTotalNrOfShots;
				}

				nrOfShotsLeftToLoad -= firingMode->nrOfShotsLeftInClip[ammoIndex];

				if(reloadTimeFraction > clipFraction && nrOfShotsLeftToLoad > 0)
				{
					firingMode->nrOfShotsLeftInClip[ammoIndex]++;
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

	//--------------------------------------------------------------------------------------
	// Drop random world pieces
	//--------------------------------------------------------------------------------------
	std::vector<int> worldPiecesIndices;
	if(nullProcessExecuting)
	{
		//--------------------------------------------------------------------------------------
		// Find all world physics objects
		//--------------------------------------------------------------------------------------
		while(itrPhysics.hasNext())
		{
			AttributePtr<Attribute_Physics> ptr_physics = itrPhysics.getNext();
			if(ptr_physics->collisionFilterGroup == XKILL_Enums::PhysicsAttributeType::WORLD)
			{
				worldPiecesIndices.push_back(ptr_physics.index());
			}
		}

		//--------------------------------------------------------------------------------------
		// Determine which world physics objects to drop and the drop ratio
		//--------------------------------------------------------------------------------------
		unsigned int nrOfWorldPieces = levelEvents_.size();
		float makeThisManyWorldPiecesFallEachSecond = nrOfWorldPieces/SETTINGS->nullprocessExecutionTime;
		float deltaRatio = 1.0f / makeThisManyWorldPiecesFallEachSecond;
		static float timer = 0;
		timer += delta;
		while(timer > deltaRatio)
		{
			if(worldPiecesIndices.size() <= 0)
			{
				break;
			}

			int randomWorldPieceIndex = rand()%worldPiecesIndices.size();

			//--------------------------------------------------------------------------------------
			// Convert world physics object to prop physics object
			//--------------------------------------------------------------------------------------
			AttributePtr<Attribute_Physics> ptr_physics;
			ptr_physics = itrPhysics.at(worldPiecesIndices.at(randomWorldPieceIndex));
			ptr_physics->collisionFilterGroup = XKILL_Enums::PhysicsAttributeType::PROP;
			ptr_physics->collisionFilterMask = XKILL_Enums::PhysicsAttributeType::NOTHING;
			ptr_physics->collisionResponse = false;
			ptr_physics->gravity = Float3(0.0f, -10.0f, 0.0f);
			ptr_physics->mass = 1;

			SEND_EVENT(&Event_ReloadPhysicsAttributeDataIntoBulletPhysics(ptr_physics.index()));

			worldPiecesIndices.at(randomWorldPieceIndex) = worldPiecesIndices.back();
			worldPiecesIndices.pop_back();

			//drop one
			timer -= deltaRatio;
		}
	}
}
#include <xkill-utilities/Converter.h>
void GameComponent::updatePlayerAttributes(float delta)
{
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
			// Ammunition logic: Ammunition change, firing mode change.
			//--------------------------------------------------------------------------------------
			bool ammunitionSwitchReload = false;
			if(ptr_input->changeAmmunitionType != 0)
			{
				//ptr_weaponStats->currentAmmunitionType = static_cast<XKILL_Enums::AmmunitionType>(nextAmmunitonTypeIndex);
				bool successfullySwitchedAmmunition = switchAmmunition(ptr_weaponStats, ptr_input->changeAmmunitionType);
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

			if(ptr_input->changeFiringMode != 0)
			{
				//ptr_weaponStats->currentFiringModeType = static_cast<XKILL_Enums::FiringModeType>(nextFiringModeIndex);
				bool successfullySwitcheFiringModeOrAmmunnition = switchFiringMode(ptr_weaponStats, ptr_input->changeFiringMode);
				if(successfullySwitcheFiringModeOrAmmunnition)
				{
					firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];
					ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];

					if(firingMode->nrOfShotsLeftInClip[ptr_weaponStats->currentAmmunitionType] > ammo->currentTotalNrOfShots)
					{
						firingMode->nrOfShotsLeftInClip[ptr_weaponStats->currentAmmunitionType] = ammo->currentTotalNrOfShots;
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
				ammo->isReloading = true;

				if(ammo->canShootWhileReloading)
				{
					int nrOfShotsToLoad = firingMode->clipSize;
					if(firingMode->clipSize > ammo->currentTotalNrOfShots)
					{
						nrOfShotsToLoad = ammo->currentTotalNrOfShots;
					}

					float reloadTimeFraction = (1.0f - (static_cast<float>(firingMode->nrOfShotsLeftInClip[ammoIndex])/static_cast<float>(nrOfShotsToLoad)));
					firingMode->reloadTimeLeft = reloadTimeFraction * firingMode->reloadTime;
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
					&& firingMode->nrOfShotsLeftInClip[ammoIndex] > 0 && (!ammo->isReloading || ammo->canShootWhileReloading))
				{
					firingMode->cooldownLeft = firingMode->cooldownBetweenShots;
					ammo->currentTotalNrOfShots--;
					firingMode->nrOfShotsLeftInClip[ammoIndex]--;

					ammo->isReloading = false;

					shootProjectile(ptr_player->ptr_weaponFireLocation_spatial, ptr_weaponStats);
					SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_FIRE, itrPlayer.ownerIdAt(ptr_player.index()), ptr_position->position, true));
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
			updateAndInterpretAimingRay(playerEntity, ptr_camera);
			
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
			else if(!nullProcessExecuting)
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
				
				//Point camera towards center
				Float3 pos2d(-ptr_position->position.x, 0.0f, -ptr_position->position.z);
				if(pos2d.length() > 0.1)
				{
					ptr_camera->up = Float3(0.0f, 1.0f, 0.0f);
					ptr_camera->look = Float3(-ptr_position->position.x, 0.0f, -ptr_position->position.z).normalize();
					ptr_camera->right = ptr_camera->up.cross(ptr_camera->look);
				
					DirectX::XMVECTOR eye,lookat,up,quat;
					DirectX::XMMATRIX rotation;
					DirectX::XMFLOAT4 quaternion;
				
					up = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0,1,0));
					eye = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(ptr_position->position.asFloat()));
					lookat = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0,0,0));
				
					rotation = DirectX::XMMatrixLookAtLH(eye,lookat,up);
					quat = DirectX::XMQuaternionRotationMatrix(rotation);
					DirectX::XMStoreFloat4(&quaternion,quat);

					ptr_spatial->rotation = Float4(quaternion.x,quaternion.y,quaternion.z,quaternion.w);
				}
				else
				{
					ptr_spatial->rotation = Float4(0.0f, 0.0f, 0.0f, 1.0f);
					ptr_camera->up = Float3(0.0f, 1.0f, 0.0f);
					ptr_camera->right = Float3(1.0f, 0.0f, 0.0f);
					ptr_camera->look = Float3(0.0f, 0.0f, 1.0f);
				}
				ptr_physics->reloadDataIntoBulletPhysics = true;
				
				ptr_health->health = ptr_health->maxHealth; // restores player health

				MutatorSettings ms;
				for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
				{
					for(int j = 0; j < XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES; j++)
					{
						ms.setupAttribute(ptr_weaponStats, static_cast<XKILL_Enums::AmmunitionType>(i), static_cast<XKILL_Enums::FiringModeType>(j));
					}
				}

				ptr_player->ptr_camera->fieldOfView = 3.14f/4.0f;
				ptr_player->respawnTimer.resetTimer();
				ptr_player->detectedAsDead = false;
				SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_RESPAWN, itrPlayer.ownerIdAt(ptr_player.index()), ptr_position->position, true));
			}
		}

		//--------------------------------------------------------------------------------------
		// Respawn player
		//--------------------------------------------------------------------------------------
		if(ptr_input->firePressed && ptr_player->detectedAsDead && ptr_player->isScoreBoardVisible)
		{
			ptr_player->respawnTimer.zeroTimer();
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

	CollisionManager* collisionManager = CollisionManager::Instance();

	collisionManager->collision_applyDamage(entity1, entity2);
	collisionManager->collision_projectile(entity1, entity2);
	collisionManager->collision_pickupable(entity1, entity2);
	collisionManager->collision_playerVsExplosionSphere(entity1, entity2);
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

	//check
	while(itrPickupable.hasNext())
	{
		itrPickupable.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPickupable.ownerId()));
	}
	//check
	while(itrPickupablesSpawnPoint.hasNext())
	{
		itrPickupablesSpawnPoint.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrPickupablesSpawnPoint.ownerId()));
	}

	

	// Show
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
		switchFiringMode(ptr_weaponStats, 0);	//Ensure ammunition disablement (selected from menu)
		
		//SEND_EVENT(&Event_HackActivated(5000.0f, XKILL_Enums::HackType::JETHACK, ptr_player)); //check jetpack giveaway
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

	ptr_player->respawnTimer.resetTimer();
	ptr_player->detectedAsDead = true;

	bool recalculateLocalInertia = true;
	SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::IF_TRUE_RECALCULATE_LOCAL_INERTIA_ELSE_SET_TO_ZERO, static_cast<void*>(&recalculateLocalInertia), ptr_physics));

	Float3 position = ptr_player->ptr_render->ptr_spatial->ptr_position->position;
	SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_DEATH, itrPlayer.ownerIdAt(e->playerAttributeIndex), position, true));

	if(ptr_player->executing)
	{
		SEND_EVENT(&Event_StopSound(XKILL_Enums::Sound::SOUND_LASER, itrPlayer.ownerIdAt(e->playerAttributeIndex)));
		ptr_player->executing = false;
	}
	if(ptr_player->jetHackPair.first)
	{
		SEND_EVENT(&Event_StopSound(XKILL_Enums::Sound::SOUND_JETPACK, itrPlayer.ownerIdAt(e->playerAttributeIndex)));
		ptr_player->jetHackPair.first = false;
	}
	if(ptr_player->cycleHackPair.first)
	{
		ptr_player->cycleHackPair.first = false;
	}
	if(ptr_player->speedHackPair.first)
	{
		ptr_player->speedHackPair.first = false;
	}
}

void GameComponent::event_UnloadLevel()
{
	std::vector<Event*>::iterator it = levelEvents_.begin();
	for(; it != levelEvents_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	/*while(itrMesh.hasNext())
	{
		itrMesh.getNext();
		SEND_EVENT(&Event_RemoveEntity(itrMesh.ownerId()));
	}*/

	levelEvents_.clear();
}

bool GameComponent::switchAmmunition(AttributePtr<Attribute_WeaponStats> ptr_weaponStats, int nrOfStepsToSwitch)
{
	bool switchedAmmunition = false;
	FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

	for(int i = 0; i < XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES; i++)
	{
		int nextAmmunitionIndex = (ptr_weaponStats->currentAmmunitionType + nrOfStepsToSwitch);
		while(nextAmmunitionIndex < 0)
			nextAmmunitionIndex += XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES;

		nextAmmunitionIndex %= XKILL_Enums::AmmunitionType::NROFAMMUNITIONTYPES;

		ptr_weaponStats->currentAmmunitionType = static_cast<XKILL_Enums::AmmunitionType>(nextAmmunitionIndex);

		//Try switching of ammunition
		if(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::BULLET && firingMode->canShootBullet || 
			ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::SCATTER && firingMode->canShootScatter || 
			ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE && firingMode->canShootExplosive)
		{
			switchedAmmunition = true;
			break;
		}
		else
		{
			nrOfStepsToSwitch = 1; //Couldn't switch that many steps, try next one until we find one that works
		}
	}

	return switchedAmmunition;
}

bool GameComponent::switchFiringMode(AttributePtr<Attribute_WeaponStats> ptr_weaponStats, int nrOfStepsToSwitch)
{
	bool switchedFiringMode = false;

	FiringMode* firingMode;

	for(int i = 0; i < XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES; i++)
	{
		int nextFiringModeIndex = (ptr_weaponStats->currentFiringModeType + nrOfStepsToSwitch);
		while(nextFiringModeIndex < 0)
			nextFiringModeIndex += XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES;

		nextFiringModeIndex %= XKILL_Enums::FiringModeType::NROFFIRINGMODETYPES;

		ptr_weaponStats->currentFiringModeType = static_cast<XKILL_Enums::FiringModeType>(nextFiringModeIndex);

		firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

		//Try switching of firing mode
		if((ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::BULLET && firingMode->canShootBullet) ||
			(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::SCATTER && firingMode->canShootScatter) ||
			(ptr_weaponStats->currentAmmunitionType == XKILL_Enums::AmmunitionType::EXPLOSIVE && firingMode->canShootExplosive))
		{
			switchedFiringMode = true;
			break;
		}
		else if(switchAmmunition(ptr_weaponStats, 1)) //switch firing mode AND ammunition
		{
			switchedFiringMode = true;
			break;
		}
		else
		{
			nrOfStepsToSwitch = 1; //Couldn't switch that many steps, try next one until we find one that works
		}
	}

	return switchedFiringMode;
}

void GameComponent::updateAndInterpretAimingRay(Entity* rayCastingPlayerEntity, AttributePtr<Attribute_Camera> ptr_camera)
{
	//--------------------------------------------------------------------------------------
	// Camera look vector ray test to far plane z. Send ray test event handled by PhysicsComponent.
	//--------------------------------------------------------------------------------------
	Float3 lookAtFarPlaneHorizon = ptr_camera->look;
	lookAtFarPlaneHorizon.normalize();
	lookAtFarPlaneHorizon = lookAtFarPlaneHorizon * ptr_camera->zFar;

	Float3 rayOrigin = ptr_camera->ptr_spatial->ptr_position->position;
	Float3 rayDestination = lookAtFarPlaneHorizon + rayOrigin;

	short rayCollisionFilterMask = XKILL_Enums::PhysicsAttributeType::PLAYER | XKILL_Enums::PhysicsAttributeType::WORLD | XKILL_Enums::PhysicsAttributeType::PICKUPABLE;
	Event_AllHitsRayCast event_AllHitsRayCast(rayOrigin, rayDestination, rayCollisionFilterMask);
	SEND_EVENT(&event_AllHitsRayCast);

	//--------------------------------------------------------------------------------------
	// Handle result from sent event to find a valid hit point for the casted ray
	//--------------------------------------------------------------------------------------
	Float3 hitPoint;
	Entity* entityHitByRay = nullptr;
	int nrOfHits = event_AllHitsRayCast.mapHitPointToEntityId.size();
	if(nrOfHits > 0) //If ray hit something
	{
		int hitEntityId = 0;
		for(int i=0;i<nrOfHits;i++)
		{
			hitEntityId = event_AllHitsRayCast.mapHitPointToEntityId.at(i).second;
			if(hitEntityId == rayCastingPlayerEntity->getID())
			{
				continue; //If player hit himself, take next hit point
			}
			else
			{
				hitPoint = event_AllHitsRayCast.mapHitPointToEntityId.at(i).first;
				entityHitByRay = &allEntity->at(hitEntityId);
				break; //Valid hit point found
			}
		}
	}
	else //If ray did not hit anything, set hitpoint to camera look far plane z
	{
		hitPoint = rayDestination;
	}
	
	//--------------------------------------------------------------------------------------
	// Update weapon rotation and ray attribute
	//--------------------------------------------------------------------------------------
	std::vector<int> rayCastingPlayerAttributeId = rayCastingPlayerEntity->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int i=0;i<rayCastingPlayerAttributeId.size();i++)
	{
		AttributePtr<Attribute_Player> rayCastingPlayerAttribute = itrPlayer.at(rayCastingPlayerAttributeId.at(i));	
		
		//--------------------------------------------------------------------------------------
		// Rotate weapon. Set weapon rotation depending on how far away the aiming ray hit something (closestHitPoint)
		//--------------------------------------------------------------------------------------
		Float3 playerLookDirection = hitPoint - ptr_camera->ptr_spatial->ptr_position->position;
		playerLookDirection.normalize();

		Float3 from = rayCastingPlayerAttribute->ptr_weapon_offset->ptr_spatial->ptr_position->position;

		//Float3 weaponLookDirection = hitPoint - from;
		//weaponLookDirection.normalize();

		Float4 newWeaponRotationQuaternion;
		newWeaponRotationQuaternion = newWeaponRotationQuaternion.quaternionLookAt(hitPoint, from);
		newWeaponRotationQuaternion.normalize();

		rayCastingPlayerAttribute->ptr_weapon_offset->ptr_spatial->rotation = newWeaponRotationQuaternion.quaternionInverse(); //Set weapon rotation

		// Update offset
		itrOffset = ATTRIBUTE_MANAGER->offset.getIterator();
		while(itrOffset.hasNext())
		{
			AttributePtr<Behavior_Offset> ptr_offset = itrOffset.getNext();
			ptr_offset->updateOffset();
		}

		//--------------------------------------------------------------------------------------
		// Update ray attribute
		//-------------------------------------------------------------------------------------
		AttributePtr<Attribute_Ray> ray;
		std::vector<int> rayAttributeId = rayCastingPlayerEntity->getAttributes(ATTRIBUTE_RAY);
		for(unsigned int j=0;j<rayAttributeId.size();j++)
		{
			ray = itrRay.at(rayAttributeId.at(j));

			//--------------------------------------------------------------------------------------
			// If the player is executing, interpret the aiming ray as a Laser Automatic Sniper Execution Ray
			//--------------------------------------------------------------------------------------
			if(rayCastingPlayerAttribute->executing && !rayCastingPlayerAttribute->detectedAsDead) 
			{
				updateAndInterpretLaser(ray, rayCastingPlayerAttribute, ptr_camera);
				ray->ptr_render->culling.values[0] = INT_MAX;
				ray->ptr_render->culling.values[1] = INT_MAX;
				ray->ptr_render->culling.values[2] = INT_MAX;
				ray->ptr_render->culling.values[3] = INT_MAX;
			}
			//--------------------------------------------------------------------------------------
			// Interpret the ray as what the player is looking at
			//--------------------------------------------------------------------------------------
			else
			{
				ray->ptr_render->culling.clear();
				//entityHitByRay might be used here (2013-02-28 17.24)
			}
		}
	}
}

void GameComponent::updateAndInterpretLaser(AttributePtr<Attribute_Ray> ptr_ray, AttributePtr<Attribute_Player> ptr_player, AttributePtr<Attribute_Camera> ptr_camera)
{
	Float3 weaponLook = Float3(0.0f, 0.0f, 1.0f).rotateWithQuaternion(ptr_player->ptr_weapon_offset->ptr_spatial->rotation);
	weaponLook.normalize();
	weaponLook = weaponLook * ptr_camera->zFar;

	Float3 from = ptr_player->ptr_weapon_offset->ptr_spatial->ptr_position->position;
	Float3 to = weaponLook + from;

	short rayCollisionFilterMask = XKILL_Enums::PhysicsAttributeType::PLAYER | XKILL_Enums::PhysicsAttributeType::WORLD | XKILL_Enums::PhysicsAttributeType::PICKUPABLE;
	Event_ClosestHitRayCast event_WeaponRay(from, to, rayCollisionFilterMask);
	SEND_EVENT(&event_WeaponRay);

	int entityIdOfOwnerToClosestPhysicsObjectHitByRay = event_WeaponRay.EntityIdOfOwnerToClosestPhysicsObjectHitByRay;
	Float3 closestHitPoint = event_WeaponRay.ClosestHitPoint;
	Entity* entityHitByRay = &allEntity->at(entityIdOfOwnerToClosestPhysicsObjectHitByRay);
	if(entityIdOfOwnerToClosestPhysicsObjectHitByRay == 0)
	{
		closestHitPoint = to;
	}
	
	//--------------------------------------------------------------------------------------
	// Rotate laser
	//--------------------------------------------------------------------------------------
	ptr_ray->from = ptr_player->ptr_weaponFireLocation_spatial->ptr_position->position;
	ptr_ray->to = closestHitPoint;

	Float3 rayVector = (ptr_ray->to - ptr_ray->from);
	ptr_ray->ptr_render->ptr_spatial->ptr_position->position = rayVector/2.0f + ptr_ray->from;
	ptr_ray->ptr_render->ptr_spatial->scale = Float3(0.01f, 0.01f, rayVector.length()/2.0f);

	Float4 laserRotation;
	laserRotation = laserRotation.quaternionLookAt(closestHitPoint, ptr_ray->from);
	laserRotation.normalize();

	ptr_ray->ptr_render->ptr_spatial->rotation = laserRotation.quaternionInverse();

	std::vector<int> playerHitByRayAttributeId = entityHitByRay->getAttributes(ATTRIBUTE_PLAYER);
	for(unsigned int j=0;j<playerHitByRayAttributeId.size();j++)
	{
		if(entityIdOfOwnerToClosestPhysicsObjectHitByRay == itrPlayer.ownerIdAt(ptr_player.index())) //Ray hit the originator of the ray
		{
			DEBUGPRINT("Player hit by ray casted by himself.");
		}
		else if(entityHitByRay->hasAttribute(ATTRIBUTE_PLAYER)) //Ray hit another player
		{
			std::vector<int> hitPlayerId = entityHitByRay->getAttributes(ATTRIBUTE_PLAYER);
			for(int i=0; i<(int)hitPlayerId.size(); i++)
			{
				AttributePtr<Attribute_Player> hitPlayerAttribute = itrPlayer.at(hitPlayerId.at(i));
				if(!hitPlayerAttribute->detectedAsDead)
				{
					if(ptr_player->cycleHackPair.first)
					{
						ptr_player->cycles++;
						{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("You exterminated", hitPlayerAttribute->avatarName, "", "+1 cycle"); SEND_EVENT(&e);}
					}
					else
					{
						ptr_player->priority++;
						{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("You exterminated", hitPlayerAttribute->avatarName, "", "+1 priority"); SEND_EVENT(&e);}
					}
					{Event_PostHudMessage e("", hitPlayerAttribute); e.setHtmlMessage("Terminated by", ptr_player->avatarName); SEND_EVENT(&e);}

					SEND_EVENT(&Event_PlayerDeath(playerHitByRayAttributeId.at(j)));
				}

				SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GIVE_IMPULSE, static_cast<void*>(&(rayVector*20.0f)), hitPlayerAttribute->ptr_input->ptr_physics));
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
	// Make sure game ends properly before starting a new game
	SEND_EVENT(&Event_EndDeathmatch());

	// Set state deathmatch
	GET_STATE() = STATE_DEATHMATCH;

	// Start deathmatch; the only gamemode so far
	// we also have to specify the number of players to start with
	int numPlayers = SETTINGS->numPlayers;
	SEND_EVENT(&Event_StartDeathmatch(numPlayers));

	// Hide mouse & menu so it is not distracting from game play
	SEND_EVENT(&Event_SetMouseLock(true));

	// Make sure the correct windows are shown
	SEND_EVENT(&Event(EVENT_FOCUS_MAINWINDOW));
	SEND_EVENT(&Event_EnableHud(true));
	SEND_EVENT(&Event_EnableMenu(false));

	// Inform players about their name
	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player>			ptr_player		=	itrPlayer		.getNext();
		{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Your nickname is", ptr_player->avatarName); SEND_EVENT(&e);}
	}
}

void GameComponent::endGame()
{
	GET_STATE() = STATE_MAINMENU;
	//SEND_EVENT(&Event_EndDeathmatch());
	//SEND_EVENT(&Event_StartDeathmatch(0));	//To get a black background, for now run the game with zero players

	// Re-enable menu so the player can decide what to do next 
	SEND_EVENT(&Event_SetMouseLock(false));
	SEND_EVENT(&Event_EnableHud(false));
	SEND_EVENT(&Event_EnableMenu(true));
}
