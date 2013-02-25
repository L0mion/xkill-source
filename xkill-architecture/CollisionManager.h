#pragma once

class Entity;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void collision_stickTogether(Entity* entity1, Entity* entity2);
	void collision_applyDamage(Entity* entity1, Entity* entity2);
	void collision_pickupable(Entity* entity1, Entity* entity2);
	void collision_projectile(Entity* entity1, Entity* entity2);
	void collision_playerVsExplosionSphere(Entity* entity1, Entity* entity2);

	int getAmountAfterPickup(int currentAmount, int maxAmount, int pickupAmount);
};