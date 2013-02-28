#ifndef XKILL_PHYSICS_COLLISIONSHAPES
#define XKILL_PHYSICS_COLLISIONSHAPES

#include <map>

#include <xkill-utilities/AttributePointer.h>

template<class T>
class btAlignedObjectArray;
class btBulletWorldImporter;
class btCollisionShape;
class btVector3;
class btTransform;
class btTriangleMesh;

struct Attribute_Mesh;

class CollisionShapes
{
private:
	CollisionShapes();
	btBulletWorldImporter* importer_;  //!< the object used to import shapes from .bullet files

	btAlignedObjectArray<btCollisionShape*>* collisionShapes_;  //!< List of added collision shapes
	std::map<unsigned int, unsigned int> collisionShapesIdToIndex_;  //!< Mapping the internal collision shape index with the global mesh id

	btAlignedObjectArray<btCollisionShape*>* unusedCollisionShapes_; //!<Child shapes to compound shapes may be stored here for deallocation possibility
	btAlignedObjectArray<btTriangleMesh*>* triangleMeshDeallocation_; //!< Triangle meshes may be stored here for deallocation possibility

	btCollisionShape* defaultShape_;  //!< a default shape given if the correct one cannot be found
	btCollisionShape* frustumShape_;  //!< The shape of frustums

	void mapCollisionShapeToGlobalMeshId(int meshId, btCollisionShape* collisionShape);
	void findMappingBetweenMeshFilesAndBulletFiles();
	void deallocateImporter();

public:
	~CollisionShapes();
	btCollisionShape* getCollisionShape(unsigned int meshId);  //!< return the collisionshape of given meshID
	btCollisionShape* getFrustumShape(unsigned int cameraIndex);  //!< return the frustum of the camera given by index
	void updateFrustumShape();  //!< Synchronizes the frustum with the cameras
	void loadCollisionShapes();  //!< Reads collision shapes of level and individual objects and store them for easy access. .bullet files contain serializations of bullet collision shapes and rigid body information, exported from Maya using the Dynamic plugin or through Blender.
	void unloadCollisionShapes();	//!< Removes collision shapes
	btCollisionShape* getDefaultShape(){return defaultShape_;}

	void addBoxCollisionShape(btVector3* scaling, btTransform* transform, int meshId);
	void loadTrianglesFromMeshAsCollisionShape(AttributePtr<Attribute_Mesh> ptr_mesh);
	
	static CollisionShapes* Instance();
	static CollisionShapes* instance;

	btCollisionShape* scatterProjectileCollisionShape;
	btCollisionShape* playerCollisionShape;
};

#endif //XKILL_PHYSICS_PHYSICSUTILITIES