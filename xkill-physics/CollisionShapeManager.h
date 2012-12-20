#ifndef XKILL_PHYSICS_COLLISIONSHAPEMANAGER
#define XKILL_PHYSICS_COLLISIONSHAPEMANAGER

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <map>

//! CollisionShapeManager
/*!
Responsible for creating collision shapes and storing them for
the physics component.
\ingroup xkill-physics
*/
struct MeshAttribute;

class CollisionShapeManager
{
private:
	std::map<unsigned int, unsigned int> collisionShapesIDtoIndex_;
	btAlignedObjectArray<btCollisionShape*> collisionShapes_; //!< A vector for holding all collision shapes
	std::vector<MeshAttribute>* meshAttributes_;
	
	std::vector<btTriangleMesh*> triangleMeshes_;
public:
	//! Creates a empty CollisionShapeManager
	CollisionShapeManager();
	//! Destroys the CollisionShapeManager and all collisionShapes within
	~CollisionShapeManager();
	//! Destroys all collisionshapes within
	void clean();
	//! Get a pointer to a specific CollisionShape
	/*
	\param index The meshID of the desired CollisionShape
	*/
	btCollisionShape* getCollisionShape(unsigned int meshID);

	//! Get a pointer to a CollisionShape at index
	/*
	\param index The index of the desired CollisionShape
	*/
	btCollisionShape* getCollisionShapeAt(unsigned int index);

	//! Create a CollisionShape for a triangle mesh. Deletes unneeded detail vertices
	/*
	\param vertices A pointer to a list of 3 float groups
	\param numVertices The number of vertices sent in
	*/
	btCollisionShape* loadCollisionShape(unsigned int meshID);

	//! Adds a collision shape
	/*
	\param the btCollisionShape to add
	*/
	void addCollisionShape(btCollisionShape* collisionShape);
};

#endif //XKILL_PHYSICS_COLLISIONSHAPEMANAGER