#ifndef XKILL_RENDERER_MESHSUBSET_H
#define XKILL_RENDERER_MESHSUBSET_H

#include <vector>
#include <string>

#include "dllUtilities.h"

//! Subset denoting part of the mesh.
/*!
Corresponds to a single MeshMaterial.
\ingroup xkill-mesh
*/
class DLL_U MeshSubset
{
public:
	//! Does nothing.
	MeshSubset();
	//! Initializes MeshSubset to arguments passed.
	MeshSubset(
		const unsigned int materialIndex,
		const std::vector<unsigned int> indices);
	//! Does nothing.
	~MeshSubset();

	const unsigned int getMaterialIndex();
	const unsigned int getNumIndices();
	const std::vector<unsigned int>& getIndices();
protected:
private:
	unsigned int materialIndex_;		//!< Index of MeshMaterial subset corresponds to.
	std::vector<unsigned int> indices_;	//!< Indices making out subset.
};

#endif //XKILL_RENDERER_MESHSUBSET_H