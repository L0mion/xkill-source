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
	btAlignedObjectArray<btCollisionShape*>* collisionShapes_;  //!< a list of collision shapes
	btCollisionShape* defaultShape_;  //!< a default shape given if the correct one cannot be found
	btCollisionShape* frustumShape_;  //!< The shape of frustums
	btBulletWorldImporter* importer_;  //!< the object used to import shapes from .bullet
	std::map<unsigned int, unsigned int> collisionShapesIdToIndex_;  //!< Mapping the internal collisionshapeindex with meshid
public:
	~CollisionShapes();
	btCollisionShape* getCollisionShape(unsigned int meshId);  //!< return the collisionshape of given meshID
	btCollisionShape* getFrustumShape(unsigned int cameraIndex);  //!< return the frustum of the camera given by index
	void updateFrustumShape();  //!< Synchronizes the frustum with the cameras
	void loadCollisionShapes();  //!< Reads collision shapes of level and individual objects and store them for easy access
	void unloadCollisionShapes();	//!< Removes collision shapes
	btCollisionShape* getDefaultShape(){return defaultShape_;}
	static CollisionShapes* Instance();
	static CollisionShapes* instance;

	btCollisionShape* scatterProjectileCollisionShape;
};

#endif //XKILL_PHYSICS_PHYSICSUTILITIES