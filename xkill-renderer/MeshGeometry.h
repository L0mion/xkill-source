#ifndef XKILL_RENDERER_MESHGEOMETRY_H
#define XKILL_RENDERER_MESHGEOMETRY_H

#include <vector>

class MeshSubset;

template <typename Vertex>
class MeshGeometry
{
public:
	MeshGeometry(
		const std::vector<Vertex>		vertices,
		const std::vector<MeshSubset>	subsets)
	{
		vertices_	= vertices;
		subsets_	= subsets;
	}
	~MeshGeometry()
	{
		//Do nothing.
	}
	
	const std::vector<Vertex>		getVertices()	{ return vertices_;	}
	const std::vector<MeshSubset>	getSubsets()	{ return subsets_;	}
protected:
private:
	std::vector<Vertex>		vertices_;
	std::vector<MeshSubset>	subsets_;
};

#endif //XKILL_RENDERER_MESHGEOMETRY_H