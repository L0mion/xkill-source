#ifndef XKILL_PHYSICS_COLLISIONSHAPEMANAGER
#define XKILL_PHYSICS_COLLISIONSHAPEMANAGER

#include <btBulletDynamicsCommon.h>

//! CollisionShapeManager
/*!
Responsible for creating collision shapes and storing them for
the physics component.
\ingroup xkill-physics
*/
class CollisionShapeManager
{
private:
	btAlignedObjectArray<btCollisionShape*> collisionShapes_; //!< An vector for holding all collision shapes
public:
	//! Creates a empty CollisionShapeManager
	CollisionShapeManager();
	//! Destroys the CollisionShapeManager and all collisionShapes within
	~CollisionShapeManager();
	//! Get a pointer to a specific CollisionShape
	/*
	\param index The index of the desired CollisionShape
	*/
	btCollisionShape* getCollisionShape(unsigned int index);
	//! Create a CollisionShape for a triangle mesh. Deletes unneeded detail vertices
	/*
	\param vertices A pointer to a list of 3 float groups
	\param numVertices The number of vertices sent in
	*/
	void createConvexHull(float* verticeData,unsigned int numVertices);
};

#endif //XKILL_PHYSICS_COLLISIONSHAPEMANAGER