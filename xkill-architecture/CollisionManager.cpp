#include "CollisionManager.h"

#include <xkill-utilities/Entity.h>
#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/Converter.h>

ATTRIBUTES_DECLARE_ALL;

CollisionManager::CollisionManager()
{
	ATTRIBUTES_INIT_ALL;
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
										creatorOfProjectile_ptr_player->priority++;
										{Event_PostHudMessage e("", creatorOfProjectile_ptr_player); e.setHtmlMessage("You terminated", "???", "", "+1 priority"); SEND_EVENT(&e);}
										{Event_PostHudMessage e("", playerThatDied_ptr_player); e.setHtmlMessage("Terminated by", "???"); SEND_EVENT(&e);}
									}
									else //Punish player for blowing himself up
									{
										creatorOfProjectile_ptr_player->priority--;
									}
									DEBUGPRINT("Player with entity id " << damage->owner_entityID << " killed player with entity id " << entity1->getID());
								}
							}
						}
					}
					else
					{
						SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_HIT, position, use3DAudio));
					}
					SEND_EVENT(&Event_Rumble(entity1->getID(), true, 0.2f, 1.0f, 1.0f));
					DEBUGPRINT("DAMAGEEVENT Entity " << entity2->getID() << " damage: " <<  damage->damage << " Entity " << entity1->getID() << " health " << health->health);
				}

				if(entity2->hasAttribute(ATTRIBUTE_PROJECTILE))
				{
					// Disarm projectile
					entity2->removeAttribute(ATTRIBUTE_DAMAGE);

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
				AttributePtr<Attribute_WeaponStats>	ptr_weaponStats	=ptr_player->ptr_weaponStats;

				Ammunition* ammo = &ptr_weaponStats->ammunition[ptr_weaponStats->currentAmmunitionType];
				FiringMode* firingMode = &ptr_weaponStats->firingMode[ptr_weaponStats->currentFiringModeType];

				//Retrieve pickupable attribute
				std::vector<int> pickupablesId = entity1->getAttributes(ATTRIBUTE_PICKUPABLE);
				for(unsigned i=0;i<pickupablesId.size();i++)
				{
					ptr_pickupable = itrPickupable.at(pickupablesId.at(i));
					std::string str_amount = Converter::IntToStr(ptr_pickupable->amount);
					switch(ptr_pickupable->pickupableType)
					{
					case XKILL_Enums::PickupableType::MEDKIT:
						{
							AttributePtr<Attribute_Health> ptr_health = ptr_player->ptr_health;
							ptr_health->health = getAmountAfterPickup(ptr_health->health, ptr_health->maxHealth, ptr_pickupable->amount);
							pickedUp = true;
							// Post HUD message
							{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Picked up", "HealthPatch", "", "+" + str_amount); SEND_EVENT(&e);}
							break;
						}
					case XKILL_Enums::PickupableType::AMMUNITION_BULLET:
						{
							AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
							weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::BULLET].currentTotalNrOfShots = getAmountAfterPickup(weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::BULLET].currentTotalNrOfShots, weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::BULLET].initialTotalNrOfShots, ptr_pickupable->amount);
							pickedUp = true;
							// Post HUD message
							{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Picked up", "Bullet Ammunition", "", "+" + str_amount); SEND_EVENT(&e);}
							break;
						}
					case XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE:
						{
							AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
							weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::EXPLOSIVE].currentTotalNrOfShots = getAmountAfterPickup(weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::EXPLOSIVE].currentTotalNrOfShots, weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::EXPLOSIVE].initialTotalNrOfShots, ptr_pickupable->amount);
							pickedUp = true;
							// Post HUD message
							{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Picked up", "Explosive Ammunition", "", "+" + str_amount); SEND_EVENT(&e);}
							break;
						}
					case XKILL_Enums::PickupableType::AMMUNITION_SCATTER:
						{
							AttributePtr<Attribute_WeaponStats> weaponStatsAttribute = ptr_player->ptr_weaponStats;
							weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::SCATTER].currentTotalNrOfShots = getAmountAfterPickup(weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::SCATTER].currentTotalNrOfShots, weaponStatsAttribute->ammunition[XKILL_Enums::AmmunitionType::SCATTER].initialTotalNrOfShots, ptr_pickupable->amount);
							pickedUp = true;
							// Post HUD message
							{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Picked up", "Scatter Ammunition", "", "+" + str_amount); SEND_EVENT(&e);}
							break;
						}
					case XKILL_Enums::PickupableType::HACK_SPEEDHACK:
						{
							pickedUp = true;
							float time = static_cast<float>(ptr_pickupable->amount);
							time /= 1000.0f;
							SEND_EVENT(&Event_HackActivated(time, XKILL_Enums::HackType::SPEEDHACK, ptr_player));
							// Post HUD message
							{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Picked up", "Speedhack", "", "+" + str_amount + "seconds"); SEND_EVENT(&e);}
							break;
						}
					case XKILL_Enums::PickupableType::HACK_JETHACK:
						{
							pickedUp = true;
							float time = static_cast<float>(ptr_pickupable->amount);
							time /= 1000.0f;
							SEND_EVENT(&Event_HackActivated(time, XKILL_Enums::HackType::JETHACK, ptr_player));
							{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Picked up", "Jethack", "", "+" + str_amount + "seconds"); SEND_EVENT(&e);}
							break;
						}
					}
				}
			}

			if(pickedUp)
			{
				// Decrement number of spawned pickupables for the spawnpoint that spawned the pickupable that the player picked up. Also remove it.
				AttributePtr<Attribute_PickupablesSpawnPoint> ptr_pickupablesSpawnPoint = ptr_pickupable->ptr_pickupablesSpawnPoint_creator;
				ptr_pickupablesSpawnPoint->currentNrOfExistingSpawnedPickupables--;
				ptr_pickupablesSpawnPoint->secondsSinceLastPickup = 0;

				SEND_EVENT(&Event_RemoveEntity(entity1->getID()));
			}
		}
	}
}

void CollisionManager::collision_projectile(Entity* entity1, Entity* entity2)
{
	if(entity1->hasAttribute(ATTRIBUTE_PROJECTILE) && entity2->hasAttribute(ATTRIBUTE_PHYSICS))
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
				float deathDelay = 2.0f;
				switch(ptr_projectile->ammunitionType)
				{
				case XKILL_Enums::AmmunitionType::BULLET: //Bounce off the wall
					deathDelay = 5.0f;
					if(ptr_projectile->currentLifeTimeLeft > deathDelay)
					{
						ptr_projectile->currentLifeTimeLeft = deathDelay;
						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GRAVITY, static_cast<void*>(&Float3(0.0f, -5.0f, 0.0f)), itrPhysics.at(physicsId.at(j))));
					}
					break;
				case XKILL_Enums::AmmunitionType::SCATTER: //Fall down and roll, also collide with projectiles
					deathDelay = 5.0f;
					if(ptr_projectile->currentLifeTimeLeft > deathDelay)
					{
						ptr_projectile->currentLifeTimeLeft = deathDelay;

						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::GRAVITY, static_cast<void*>(&Float3(0.0f, -10.0f, 0.0f)), itrPhysics.at(physicsId.at(j))));
						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::VELOCITYPERCENTAGE, static_cast<void*>(&Float3(0.1f, 0.1f, 0.1f)), itrPhysics.at(physicsId.at(j))));

						//Collide with projectiles
						short collisionFilterMask = itrPhysics.at(physicsId.at(j))->collisionFilterMask | XKILL_Enums::PhysicsAttributeType::PROJECTILE;
						SEND_EVENT(&Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::COLLISIONFILTERMASK, static_cast<void*>(&collisionFilterMask), itrPhysics.at(physicsId.at(j))));
					
						// MATT: This makes the initial collision smoother by adding a delay of one frame.
						// ON A SECOND NOTE: Not used, because delaying collision makes bullets rain from the sky.
						//POST_DELAYED_EVENT(new Event_ModifyPhysicsObject(XKILL_Enums::ModifyPhysicsObjectData::COLLISIONFILTERMASK, static_cast<void*>(&collisionFilterMask), itrPhysics.at(physicsId.at(j))), 0.0f);
					}
					break;
				case XKILL_Enums::AmmunitionType::EXPLOSIVE: //Remove projectile and create an explosion sphere in its place
					{
						ptr_projectile->currentLifeTimeLeft = 0.0f; //Kill the projectile that caused the explosion

						//Extract projectile position.
						AttributePtr<Attribute_Physics> ptr_projectile_physics	 = ptr_projectile->ptr_physics;
						AttributePtr<Attribute_Spatial> ptr_projectile_spatial	 = ptr_projectile_physics->ptr_spatial;
						AttributePtr<Attribute_Position> ptr_projectile_position = ptr_projectile_spatial->ptr_position;

						//Creates an explosion sphere. Init information is taken from the impacting projectile.
						SEND_EVENT(&Event_CreateExplosionSphere(ptr_projectile_position->position, 1, ptr_projectile->entityIdOfCreator, ptr_projectile->ammunitionType, ptr_projectile->firingModeType));
						break;
					}
				default:
					ERROR_MESSAGEBOX("PhysicsAttribute collision: unknown ammunitionType"); 
					break;
				}
			}
		}
		//SEND_EVENT(&Event_RemoveEntity(entity1->getID())); //Crashes sometimes if removed here
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
			}
		}
	}
}

int CollisionManager::getAmountAfterPickup(int currentAmount, int maxAmount, int pickupAmount)
{
	int newAmount = currentAmount;
	if(currentAmount < maxAmount)					//Pickup if currently below maximum amount
	{
		newAmount = currentAmount + pickupAmount;	//Pickup
		if(newAmount > maxAmount)
		{
			newAmount = maxAmount;					//Prevent picking up more than maximum
		}
	}
	return newAmount;
}