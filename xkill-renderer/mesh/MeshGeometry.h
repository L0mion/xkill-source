#ifndef XKILL_RENDERER_MESHGEOMETRY_H
#define XKILL_RENDERER_MESHGEOMETRY_H

#include <vector>

#include "MeshSubset.h"
#include "vertices.h"

//! Encapsulates geometric data of a mesh.
/*!
Encapsulates vertices and subsets.
\ingroup xkill-mesh
*/
class MeshGeometry
{
public:
	//! Does nothing.
	MeshGeometry();
	//! Initializes MeshGeometry to data passed through constructor.
	MeshGeometry(
		const std::vector<VertexPosNormTex>	vertices,
		const std::vector<MeshSubset>		subsets);
	~MeshGeometry();
	
	std::vector<VertexPosNormTex>	getVertices();
	std::vector<MeshSubset>			getSubsets();

	unsigned int getNumVertices();
	unsigned int getNumSubsets();
protected:
private:
	std::vector<VertexPosNormTex>	vertices_;	//!< Vertices of mesh.
	std::vector<MeshSubset>			subsets_;	//!< Subsets of mesh. \sa MeshSubset
};

#endif //XKILL_RENDERER_MESHGEOMETRY_H