#ifndef XKILL_RENDERER_MESHGEOMETRY_H
#define XKILL_RENDERER_MESHGEOMETRY_H

#include <vector>

#include "MeshSubset.h"
#include "vertices.h"

class MeshGeometry
{
public:
	MeshGeometry();
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
	std::vector<VertexPosNormTex>	vertices_;
	std::vector<MeshSubset>			subsets_;
};

#endif //XKILL_RENDERER_MESHGEOMETRY_H