#ifndef XKILL_PHYSICS_COLLISIONSHAPES
#define XKILL_PHYSICS_COLLISIONSHAPES

#include <map>

template<class T>
class btAlignedObjectArray;
class btBulletWorldImporter;
class btCollisionShape;

class CollisionShapes
{
private:
	CollisionShapes();
	btAlignedObjectArray<btCollisionShape*>* collisionShapes_;
	btCollisionShape* defaultShape_;
	btBulletWorldImporter* importer_;
	std::map<unsigned int, unsigned int> collisionShapesIdToIndex_;
public:
	~CollisionShapes();
	btCollisionShape* getCollisionShape(unsigned int meshId);
	void loadWorldCollisionShapes(std::string world);
	void loadCollisionShapes();
	static CollisionShapes* Instance();
	static CollisionShapes* instance;
};

#endif //XKILL_PHYSICS_PHYSICSUTILITIES