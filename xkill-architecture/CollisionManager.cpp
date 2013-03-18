#include "CollisionManager.h"

#include <xkill-utilities/Entity.h>
#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/Converter.h>
#include <xkill-utilities/MutatorSettings.h>

CollisionManager* CollisionManager::instance = nullptr;
ATTRIBUTES_DECLARE_ALL;

CollisionManager::CollisionManager()
{
	instance = nullptr;
	ATTRIBUTES_INIT_ALL;
}

CollisionManager* CollisionManager::Instance()
{
	if(instance == nullptr)
	{
		instance = new CollisionManager();
	}
	return instance;
}

CollisionManager::~CollisionManager()
{

}


void CollisionManager::collision_stickTogether(Entity* entity1, Entity* entity2)
{
	if(entity1->hasAttribute(ATTRIBUTE_PHYSICS) && entity2->hasAttribute(ATTRIBUTE_PHYSICS))
	{
		AttributePtr<Attribute_Spatial> ptr_target_spatial = (itrPhysics.getMultiple(entity1->getAttributes(ATTRIBUTE_PHYSICS)).at(0))->ptr_spatial;
		AttributePtr<Attribute_Spatial> ptr_projectile_spatial = (itrPhysics.getMultiple(entity2->getAttributes(ATTRIBUTE_PHYSICS)).at(0))->ptr_spatial;
		entity2->removeAttribute(ATTRIBUTE_PHYSICS);

		AttributePtr<Behavior_Offset> ptr_projectile_offset = itrOffset.createAttribute(entity2);
		ptr_projectile_offset->ptr_spatial = ptr_projectile_spatial;
		ptr_projectile_offset->ptr_parent_spatial_position = ptr_target_spatial;
		ptr_projectile_offset->ptr_parent_spatial_rotation = ptr_target_spatial;

		Float3 offsetVector = ptr_projectile_spatial->ptr_position->position - ptr_target_spatial->ptr_position->position;
		ptr_projectile_offset->offset_position = offsetVector;
	}
}

void CollisionManager::collision_applyDamage(Entity* entity1, Entity* entity2)
{
	if(entity1->hasAttribute(ATTRIBUTE_HEALTH) && entity2->hasAttribute(ATTRIBUTE_DAMAGE))
	{
		// fetch damage from entity 2
		std::vector<int> damageId = entity2->getAttributes(ATTRIBUTE_DAMAGE);

		// fetch health from entity 1
		std::vector<int> healthId = entity1->getAttributes(ATTRIBUTE_HEALTH);

		for(unsigned i=0; i<damageId.size(); i++)
		{
			AttributePtr<Attribute_Damage> damage = itrDamage.at(damageId[i]);

			// avoid damage to self
			if(entity1->getID() != damage->owner_entityID || entity2->hasAttribute(ATTRIBUTE_EXPLOSIONSPHERE))
			{
				Float3 position;
				bool use3DAudio = false;

				if(entity1->hasAttribute(ATTRIBUTE_POSITION))
				{
					std::vector<int> positionID = entity1->getAttributes(ATTRIBUTE_POSITION);

					for(unsigned int i = 0; i < positionID.size(); i++)
					{
						AttributePtr<Attribute_Position> ptr_position = itrPosition.at(positionID[i]);

						position = ptr_position->position;
						use3DAudio = true;
					}
				}

				// Apply damage to all Health attributes
				for(unsigned j=0; j<healthId.size(); j++)
				{
					AttributePtr<Attribute_Health> health = itrHealth.at(healthId[j]);
					health->health -= damage->damage;

					// Send feedback to all players
					std::vector<AttributePtr<Attribute_Player>> players_takingDamage = itrPlayer.getMultiple(entity1->getAttributes(ATTRIBUTE_PLAYER));
					for(int i=0; i<(int)players_takingDamage.size(); i++)
					{
						SEND_EVENT(&Event_PlayerTakingDamage(players_takingDamage[i]));
						players_takingDamage[i]->ptr_render->ptr_animation->activeAnimation = "processHit";
						players_takingDamage[i]->ptr_render->ptr_animation->time = 0.0f;
					}
					std::vector<AttributePtr<Attribute_Player>> players_hittingTargetAs = itrPlayer.getMultiple(itr_entity->at(damage->owner_entityID)->getAttributes(ATTRIBUTE_PLAYER));
					for(int i=0; i<(int)players_hittingTargetAs.size(); i++)
					{
						SEND_EVENT(&Event_PlayerTargetHit(players_hittingTargetAs[i]));
					}

					// If a player was killed by the collision, give priority (score) to the player that created the DamageAttribute
					if(health->health <= 0)
					{
						Entity* playerThatDied = itr_entity->at(itrHealth.ownerIdAt(healthId[j]));
						std::vector<int> playerThatDiedId = playerThatDied->getAttributes(ATTRIBUTE_PLAYER);

						Entity* creatorOfProjectilePlayerEntity = itr_entity->at(damage->owner_entityID);
						std::vector<int> creatorOfProjectilePlayerId = creatorOfProjectilePlayerEntity->getAttributes(ATTRIBUTE_PLAYER);
						for(unsigned k=0;k<creatorOfProjectilePlayerId.size();k++)
						{
							for(unsigned l=0;l<playerThatDiedId.size();l++)
							{
								AttributePtr<Attribute_Player> playerThatDied_ptr_player = itrPlayer.at(playerThatDiedId.at(l));
								if(!playerThatDied_ptr_player->detectedAsDead) //Prevent player from receiving priority based on number of fatal hits
								{
									AttributePtr<Attribute_Player> creatorOfProjectile_ptr_player = itrPlayer.at(creatorOfProjectilePlayerId.at(k));
									if(entity1->getID() != damage->owner_entityID) //Award player
									{
										int reward = 1;
										if(playerThatDied_ptr_player->executing) //increase reward after fragging executing player
										{
											reward *= itrPlayer.count();
										}

										if(creatorOfProjectile_ptr_player->cycleHackPair.first) // Cyclehack active
										{
											if(playerThatDied_ptr_player->cycles > 0) //The fragged player had cycles to steal
											{
												reward = 1; //steal only 1 cycle
												playerThatDied_ptr_player->cycles -= reward;	
												creatorOfProjectile_ptr_player->cycles += reward;
												{Event_PostHudMessage e("", creatorOfProjectile_ptr_player); e.setColor(playerThatDied_ptr_player->avatarColor); e.setHtmlMessage("You terminated and stole a cycle from", playerThatDied_ptr_player->avatarName, "", "+" + Converter::IntToStr(reward) + " cycles"); SEND_EVENT(&e);}
												{Event_PostHudMessage e("", playerThatDied_ptr_player); e.setColor(creatorOfProjectile_ptr_player->avatarColor); e.setHtmlMessage("Terminated and cycle stolen by", creatorOfProjectile_ptr_player->avatarName); SEND_EVENT(&e);}
											}
											else //The fragged player did not have cycles to steal
											{
												reward = 0; //no cycles stolen
												{Event_PostHudMessage e("", creatorOfProjectile_ptr_player); e.setColor(playerThatDied_ptr_player->avatarColor); e.setHtmlMessage("You terminated and tried to steal a cycle from", playerThatDied_ptr_player->avatarName, "", "+" + Converter::IntToStr(reward) + " cycles"); SEND_EVENT(&e);}
												{Event_PostHudMessage e("", playerThatDied_ptr_player); e.setColor(creatorOfProjectile_ptr_player->avatarColor); e.setHtmlMessage("Terminated and almost cycle-stolen by", creatorOfProjectile_ptr_player->avatarName); SEND_EVENT(&e);}
											}

										}
										else //Cycle hack not active
										{
											creatorOfProjectile_ptr_player->priority += reward;
											{Event_PostHudMessage e("", creatorOfProjectile_ptr_player); e.setColor(playerThatDied_ptr_player->avatarColor); e.setHtmlMessage("You terminated", playerThatDied_ptr_player->avatarName, "", "+" + Converter::IntToStr(reward) + " priority"); SEND_EVENT(&e);}
											{Event_PostHudMessage e("", playerThatDied_ptr_player); e.setColor(creatorOfProjectile_ptr_player->avatarColor); e.setHtmlMessage("Terminated by", creatorOfProjectile_ptr_player->avatarName); SEND_EVENT(&e);}
										}
									}
									else //Punish player for blowing himself up
									{
										{Event_PostHudMessage e("", creatorOfProjectile_ptr_player); e.setHtmlMessage("You","self-terminated","", "-1 priority"); SEND_EVENT(&e);}
										if(creatorOfProjectile_ptr_player->priority > 0) //Prevent overpunishment of players
										{
											creatorOfProjectile_ptr_player->priority--;
										}
									}
									playerThatDied_ptr_player->detectedAsDead = true;
									SEND_EVENT(&Event_PlayerDeath(playerThatDiedId.at(l)));

									DEBUGPRINT("Player with entity id " << damage->owner_entityID << " killed player with entity id " << entity1->getID());
								}
							}
						}
					}
					else
					{
						SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_HIT, entity1->getID(), position, use3DAudio));
					}
					SEND_EVENT(&Event_Rumble(entity1->getID(), true, 0.2f, 1.0f, 1.0f));
					DEBUGPRINT("DAMAGEEVENT Entity " << entity2->getID() << " damage: " <<  damage->damage << " Entity " << entity1->getID() << " health " << health->health);
				}

				if(entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
				{
					SEND_EVENT(&Event_RemoveEntity(entity2->getID()));
					
					//
					// Make projectiles stick to player
					//

					//collision_stickTogether(entity1, entity2);
				}
			}
		}
	}
}

void CollisionManager::collision_pickupable(Entity* entity1, Entity* entity2)
{
	if(entity1->hasAttribute(ATTRIBUTE_PICKUPABLE))
	{
		if(entity2->hasAttribute(ATTRIBUTE_PLAYER))
		{
			AttributePtr<Attribute_Pickupable> ptr_pickupable;
			bool pickedUp = false;
			//Retrieve player attribute
			std::vector<int> playerId = entity2->getAttributes(ATTRIBUTE_PLAYER);
			for(unsigned i=0;i<playerId.size();i++)
			{
				AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerId.at(i));
				if(!ptr_player->detectedAsDead && ptr_player->ptr_health->health > 0) //If the player is alive
				{
					AttributePtr<Attribute_WeaponStats>	ptr_weaponStats	= ptr_player->ptr_weaponStats;

					Ammunition* ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
					FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

					//Retrieve pickupable attribute
					std::vector<int> pickupablesId = entity1->getAttributes(ATTRIBUTE_PICKUPABLE);
					for(unsigned i=0;i<pickupablesId.size();i++)
					{
						ptr_pickupable = itrPickupable.at(pickupablesId.at(i));

						//--------------------------------------------------------------------------------------
						// Random hack
						//--------------------------------------------------------------------------------------
						if(ptr_pickupable->pickupableType == XKILL_Enums::PickupableType::HACK_RANDOMHACK)
						{
							static int infiniteLoopCounter = 0;
							bool randomHackPickedUp = false;
							while(!randomHackPickedUp && infiniteLoopCounter < 250)
							{
								int randomPickupableTypeInt = rand()%XKILL_Enums::PickupableType::NROFPICKUPABLETYPES;
								ptr_pickupable->pickupableType = (XKILL_Enums::PickupableType)randomPickupableTypeInt;

								//--------------------------------------------------------------------------------------
								// List of all pickupables retrievable from random hack
								//--------------------------------------------------------------------------------------
								if(ptr_pickupable->pickupableType == XKILL_Enums::PickupableType::HACK_SPEEDHACK ||
								ptr_pickupable->pickupableType == XKILL_Enums::PickupableType::HACK_JETHACK ||
								ptr_pickupable->pickupableType == XKILL_Enums::PickupableType::HACK_CYCLEHACK ||
								ptr_pickupable->pickupableType == XKILL_Enums::PickupableType::HACK_POWERHACK)
								{
									randomHackPickedUp = true;
								}
								infiniteLoopCounter++;
							}
						}
			
						int amount;
						switch(ptr_pickupable->pickupableType)
						{
						case XKILL_Enums::PickupableType::MEDKIT:
							{
								amount = 20;
								AttributePtr<Attribute_Health> ptr_health = ptr_player->ptr_health;
								int pickedUpAmount = getPickedUpAmount((int)ptr_health->health, (int)ptr_health->maxHealth, amount);
								if(pickedUpAmount > 0)
								{
									ptr_health->health += pickedUpAmount;
									pickedUp = true;

									// Post HUD message
									{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "HealthPatch", "", "+" + Converter::IntToStr(pickedUpAmount)); SEND_EVENT(&e);}
								}
								break;
							}
						case XKILL_Enums::PickupableType::AMMUNITION_BULLET:
							{
								amount = 20;
								AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
								Ammunition* ammo = &weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::BULLET];
								int pickedUpAmount = getPickedUpAmount(ammo->currentTotalNrOfShots, ammo->maxTotalNrOfShots, amount);
								if(pickedUpAmount > 0)
								{
									weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::BULLET].currentTotalNrOfShots += pickedUpAmount;
									pickedUp = true;
				
									// Post HUD message
									{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Bullet Ammunition", "", "+" + Converter::IntToStr(pickedUpAmount)); SEND_EVENT(&e);}
								}
								break;
							}
						case XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE:
							{
								amount = 10;
								AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
								Ammunition* ammo = &weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::EXPLOSIVE];
								int pickedUpAmount = getPickedUpAmount(ammo->currentTotalNrOfShots, ammo->maxTotalNrOfShots, amount);
								if(pickedUpAmount > 0)
								{
									weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::EXPLOSIVE].currentTotalNrOfShots += pickedUpAmount;
									pickedUp = true;

									// Post HUD message
									{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Explosive Ammunition", "", "+" + Converter::IntToStr(pickedUpAmount)); SEND_EVENT(&e);}
								}
								break;
							}
						case XKILL_Enums::PickupableType::AMMUNITION_SCATTER:
							{
								amount = 50;
								AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
								Ammunition* ammo = &weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::SCATTER];
								int pickedUpAmount = getPickedUpAmount(ammo->currentTotalNrOfShots, ammo->maxTotalNrOfShots, amount);
								if(pickedUpAmount > 0)
								{
									weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::SCATTER].currentTotalNrOfShots += pickedUpAmount;
									pickedUp = true;

									// Post HUD message
									{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Scatter Ammunition", "", "+" + Converter::IntToStr(pickedUpAmount)); SEND_EVENT(&e);}
								}
								break;
							}
						case XKILL_Enums::PickupableType::HACK_SPEEDHACK:
							{
								amount = 5+rand()%10;
								pickedUp = true;
								float time = static_cast<float>(amount);
								SEND_EVENT(&Event_HackActivated(time, XKILL_Enums::HackType::SPEEDHACK, ptr_player));

								// Post HUD message
								{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Speedhack", "", "+" + Converter::IntToStr(amount) + " seconds"); SEND_EVENT(&e);}
								break;
							}
						case XKILL_Enums::PickupableType::HACK_JETHACK:
							{
								amount = 10+rand()%20;
								pickedUp = true;
								float time = static_cast<float>(amount);
								SEND_EVENT(&Event_HackActivated(time, XKILL_Enums::HackType::JETHACK, ptr_player));

								// Post HUD message
								{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Jethack", "", "+" + Converter::IntToStr(amount) + " seconds"); SEND_EVENT(&e);}
								break;
							}
						case XKILL_Enums::PickupableType::HACK_CYCLEHACK:
							{
								amount = 5+rand()%15;
								pickedUp = true;
								float time = static_cast<float>(amount);
								SEND_EVENT(&Event_HackActivated(time, XKILL_Enums::HackType::CYCLEHACK, ptr_player));

								// Post HUD message
								{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Cyclehack (cycle steal)", "", "+" + Converter::IntToStr(amount) + " seconds"); SEND_EVENT(&e);}
								break;
							}
						case XKILL_Enums::PickupableType::HACK_POWERHACK:
							{
								amount = 10+rand()%30;
								pickedUp = true;
								float time = static_cast<float>(amount);
								SEND_EVENT(&Event_HackActivated(time, XKILL_Enums::HackType::POWERHACK, ptr_player));

								// Post HUD message
								{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_pickupable->getColor()); e.setHtmlMessage("Picked up", "Powerhack (2x damage)", "", "+" + Converter::IntToStr(amount) + " seconds"); SEND_EVENT(&e);}
								break;
							}
						}
					}
				}
			}
			if(pickedUp)
			{
				removePickupable(ptr_pickupable);
			}
		}
	}
}

void CollisionManager::collision_projectile(Entity* entity1, Entity* entity2)
{
	if(entity1->hasAttribute(ATTRIBUTE_PROJECTILE) && entity2->hasAttribute(ATTRIBUTE_PHYSICS) && !entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
	{
		// Handle PhysicsAttribute of a projectile colliding with another PhysicsAttribute
		std::vector<int> physicsId = entity1->getAttributes(ATTRIBUTE_PHYSICS);
		for(unsigned int i=0;i<physicsId.size();i++)
		{
			std::vector<int> projectileId = entity1->getAttributes(ATTRIBUTE_PROJECTILE);
			for(unsigned j=0;j<projectileId.size();j++)
			{
				AttributePtr<Attribute_Projectile> ptr_projectile = itrProjectile.at(projectileId.at(i));

				//Determine collision effect based on ammunitionType
				float projectileLifeTimeAfterCollision = 1.0f;
				switch(ptr_projectile->ammunitionType)
				{
				case XKILL_Enums::AmmunitionType::BULLET: //Bounce off the wall
					if(ptr_projectile->currentLifeTimeLeft > projectileLifeTimeAfterCollision)
					{
						ptr_projectile->currentLifeTimeLeft = projectileLifeTimeAfterCollision;
						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GRAVITY, static_cast<void*>(&Float3(0.0f, -5.0f, 0.0f)), itrPhysics.at(physicsId.at(j))));
					}
					break;
				case XKILL_Enums::AmmunitionType::SCATTER: //Fall down and roll, also collide with projectiles
					if(ptr_projectile->currentLifeTimeLeft > projectileLifeTimeAfterCollision)
					{
						ptr_projectile->currentLifeTimeLeft = projectileLifeTimeAfterCollision;

						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GRAVITY, static_cast<void*>(&Float3(0.0f, -10.0f, 0.0f)), itrPhysics.at(physicsId.at(j))));
						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::VELOCITYPERCENTAGE, static_cast<void*>(&Float3(0.1f, 0.1f, 0.1f)), itrPhysics.at(physicsId.at(j))));

						//Collide with projectiles
						short collisionFilterMask = itrPhysics.at(physicsId.at(j))->collisionFilterMask | XKILL_Enums::PhysicsAttributeType::PROJECTILE;
						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::COLLISIONFILTERMASK, static_cast<void*>(&collisionFilterMask), itrPhysics.at(physicsId.at(j))));
					
						// MATT: This makes the initial collision smoother by adding a delay of one frame.
						// ON A SECOND NOTE: Not used, because delaying collision makes bullets rain from the sky.
						// POST_DELAYED_EVENT(new Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::COLLISIONFILTERMASK, static_cast<void*>(&collisionFilterMask), itrPhysics.at(physicsId.at(j))), 0.0f);
					}
					break;
				case XKILL_Enums::AmmunitionType::EXPLOSIVE: //Remove projectile and create an explosion sphere in its place
					{
						ptr_projectile->currentLifeTimeLeft = 0.0f; //Kill the projectile that caused the explosion

						break;
					}
				default:
					ERROR_MESSAGEBOX("PhysicsAttribute collision: unknown ammunitionType"); 
					break;
				}

				MutatorSettings ms;

				if(ms.getStandardAmmunition(ptr_projectile->ammunitionType).explosive)
				{
					//Extract projectile position.
					AttributePtr<Attribute_Physics>  ptr_projectile_physics	 = ptr_projectile->ptr_physics;
					AttributePtr<Attribute_Spatial>  ptr_projectile_spatial	 = ptr_projectile_physics->ptr_spatial;
					AttributePtr<Attribute_Position> ptr_projectile_position = ptr_projectile_spatial->ptr_position;

					ptr_projectile->currentLifeTimeLeft = 0.0f;

					//Creates an explosion sphere. Init information is taken from the impacting projectile.
					SEND_EVENT(&Event_CreateExplosionSphere(ptr_projectile_position->position, ptr_projectile->entityIdOfCreator, ptr_projectile->ammunitionType, ptr_projectile->firingModeType));
				}
			}
		}
		//SEND_EVENT(&Event_RemoveEntity(entity1->getID())); //Crashes sometimes if removed here //old comment from late 2012

		if( !(entity2->hasAttribute(ATTRIBUTE_PLAYER)) ) //If the projectile colliding with something that was not a player
		{
			entity1->removeAttribute(ATTRIBUTE_DAMAGE); // Disarm projectile			
		}
	}
}

void CollisionManager::collision_playerVsExplosionSphere(Entity* entity1, Entity* entity2)
{
	if(entity1->hasAttribute(ATTRIBUTE_EXPLOSIONSPHERE))
	{
		if(entity2->hasAttribute(ATTRIBUTE_PLAYER))
		{
			std::vector<int> explosionSphereID = entity1->getAttributes(ATTRIBUTE_EXPLOSIONSPHERE);
			for(unsigned int i = 0; i < explosionSphereID.size(); i++)
			{
				AttributePtr<Attribute_ExplosionSphere> ptr_explosionSphere = itrExplosionSphere.at(explosionSphereID.at(i));

				std::vector<int> playerID = entity2->getAttributes(ATTRIBUTE_PLAYER);
				for(unsigned int j = 0; j < explosionSphereID.size(); j++)
				{
					AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(playerID.at(j));

					Float3 playerPosition = ptr_player->ptr_render->ptr_spatial->ptr_position->position;
					Float3 explosionSpherePosition = ptr_explosionSphere->ptr_physics->ptr_spatial->ptr_position->position;

					Float3 impulseVector = playerPosition - explosionSpherePosition;

					impulseVector = impulseVector * 3.0f;

					SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GIVE_IMPULSE, static_cast<void*>(&impulseVector), ptr_player->ptr_input->ptr_physics));
				}

				std::vector<int> damageID = entity1->getAttributes(ATTRIBUTE_DAMAGE);
				for(unsigned int i = 0; i < damageID.size(); i++)
				{
					AttributePtr<Attribute_Damage> ptr_damage = itrDamage.at(damageID[i]);

					ptr_damage->damage = ptr_explosionSphere->damage * SETTINGS->trueDeltaTime * SETTINGS->timeScale();
				}
			}
		}
	}
}

void CollisionManager::removePickupable(AttributePtr<Attribute_Pickupable> ptr_pickupable)
{
	// Decrement number of spawned pickupables for the spawnpoint that spawned the pickupable that the player picked up. Also remove it.
	AttributePtr<Attribute_PickupablesSpawnPoint> ptr_pickupablesSpawnPoint = ptr_pickupable->ptr_pickupablesSpawnPoint_creator;
	ptr_pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables--;
	ptr_pickupablesSpawnPoint->secondsSinceLastPickup = 0;

	SEND_EVENT(&Event_RemoveEntity(itrPickupable.ownerIdAt(ptr_pickupable.index())));
}

int CollisionManager::getPickedUpAmount(int currentAmount, int maxAmount, int tryPickupAmount)
{
	int pickedUpAmount = 0;
	if(currentAmount < maxAmount) //Pickup if currently below maximum amount
	{
		int pickupFullAmount = currentAmount + tryPickupAmount;
		if(pickupFullAmount > maxAmount) //Max capping
		{
			int difference = pickupFullAmount - maxAmount;
			pickedUpAmount = tryPickupAmount - difference;
		}
		else //Pickup
		{
			pickedUpAmount = tryPickupAmount;
		}
	}
	return pickedUpAmount;
}