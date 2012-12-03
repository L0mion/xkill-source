#include "MeshGeometry.h"

MeshGeometry::MeshGeometry()
{
	//Get rid of this
}
MeshGeometry::MeshGeometry(
	const std::vector<VertexPosNormTex>	vertices,
	const std::vector<MeshSubset>		subsets)
{
	vertices_	= vertices;
	subsets_	= subsets;
}
MeshGeometry::~MeshGeometry()
{
	//Do nothing.
}

std::vector<VertexPosNormTex> MeshGeometry::getVertices()
{
	return vertices_;
}
std::vector<MeshSubset>	MeshGeometry::getSubsets()
{
	return subsets_;
}