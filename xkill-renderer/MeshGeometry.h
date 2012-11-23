#ifndef XKILL_RENDERER_MESHGEOMETRY_H
#define XKILL_RENDERER_MESHGEOMETRY_H

#include <vector>

#include <vertices.h>

template <typename Vertex>
class MeshGeometry
{
public:
	MeshGeometry(
		const std::vector<Vertex>			mgVertices,
		const std::vector<unsigned int>		mgIndices)
	{
		mgVertices_ = mgVertices;
		mgIndices_	= mgIndices;
	}
	~MeshGeometry()
	{
		//Do nothing.
	}

	const std::vector<Vertex>			getMGVertices()
	{
		return mgVertices_;
	}
	const std::vector<unsigned int>		getMGIndices()
	{
		return mgIndices_;
	}
protected:
private:
	std::vector<Vertex>				mgVertices_;
	std::vector<unsigned int>		mgIndices_;
};

/*
Template vertex-type?
*/

#endif //XKILL_RENDERER_MESHGEOMETRY_H