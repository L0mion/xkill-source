#ifndef XKILL_RENDERER_OBJGEOMETRY_H
#define XKILL_RENDERER_OBJGEOMETRY_H

#include <vector>

#include "ObjGroup.h"
#include <xkill-utilities/MeshVertices.h>

//! Describes geometry of an .obj-model.
/*!
\ingroup xkill-mesh-io-obj
*/
class ObjGeometry
{
public:
	//Does nothing.
	ObjGeometry();
	//Initializes geometry with vertices making out the foundation of the model.
	ObjGeometry(const std::vector<VertexPosNormTex> vertices);
	//Does nothing.
	~ObjGeometry();

	//Adds a group to geometry-object. This group dictates a subset of the vertices.
	void pushGroup(const ObjGroup mg);

	const std::vector<VertexPosNormTex>		getVertices();
	const std::vector<ObjGroup>				getObjGroups();
protected:
private:
	std::vector<VertexPosNormTex>			vertices_;	//!< Vertices of .obj-model.
	std::vector<ObjGroup>					objGroups_;	//!< Subsets of vertices, binding parts of the mesh to corresponding materials.
};

#endif //XKILL_RENDERER_OBJGEOMETRY_H