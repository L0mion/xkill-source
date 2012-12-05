#include "MeshSubset.h"

MeshSubset::MeshSubset()
{
	//Do nothing.
}
MeshSubset::MeshSubset(
	const unsigned int materialIndex,
	const std::vector<unsigned int> indices)
{
	materialIndex_	= materialIndex;
	indices_		= indices;
}
MeshSubset::~MeshSubset()
{
	//Do nothing.
}

const unsigned int MeshSubset::getMaterialIndex()
{
	return materialIndex_;
}
const unsigned int MeshSubset::getNumIndices()
{
	return indices_.size();
}
const std::vector<unsigned int>& MeshSubset::getIndices()
{
	return indices_;
}