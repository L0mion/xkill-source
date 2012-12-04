#ifndef XKILL_RENDERER_MESHMODEL_H
#define XKILL_RENDERER_MESHMODEL_H

/*! \defgroup xkill-mesh xkill-mesh
	Generalized mesh used by renderable objects in XKILL. */

#include <vector>

#include "MeshGeometry.h"
#include "MeshMaterial.h"
#include "vertices.h"

//! Generalized Model describing mesh of renderable objects.
/*!
Contains Geometry in terms of vertices and indices subdivided into subsets and material data.
\ingroup xkill-mesh
*/
class MeshModel
{
public:
	//! Does nothing.
	MeshModel();
	//! Initializes Model to default state.
	MeshModel(
		const MeshGeometry				geometry,
		const std::vector<MeshMaterial>	materials);
	//! Does nothing.
	~MeshModel();

	MeshGeometry				getGeometry();
	unsigned int				getNumMaterials();
	std::vector<MeshMaterial>	getMaterials();
protected:
private:
	MeshGeometry				geometry_;	//!< Vertex and index data of mesh.
	std::vector<MeshMaterial>	materials_;	//!< Materials describing rendering properties of subsets.
};

#endif //XKILL_RENDERER_MESHMODEL_H