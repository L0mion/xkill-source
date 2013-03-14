#pragma once

class Entity;
struct Attribute_Pickupable;
template <class T> class AttributePtr;

class CollisionManager
{
private:
	CollisionManager();
public:
	~CollisionManager();

	void collision_stickTogether(Entity* entity1, Entity* entity2);
	void collision_applyDamage(Entity* entity1, Entity* entity2);
	void collision_pickupable(Entity* entity1, Entity* entity2);
	void collision_projectile(Entity* entity1, Entity* entity2);
	void collision_playerVsExplosionSphere(Entity* entity1, Entity* entity2);

	void removePickupable(AttributePtr<Attribute_Pickupable> ptr_pickupable);
	int getPickedUpAmount(int currentAmount, int maxAmount, int tryPickupAmount);

	static CollisionManager* Instance();
	static CollisionManager* instance;
};