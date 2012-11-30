#include <MeshSubset.h>

MeshSubset::MeshSubset(
	const std::string name,
	const std::string materialName,
	const std::vector<unsigned int> indices)
{
	name_			= name;
	materialName_	= materialName;
	indices_		= indices;
}
MeshSubset::~MeshSubset()
{
	//Do nothing.
}

const std::string MeshSubset::getName()
{
	return name_;
}
const std::string MeshSubset::getMaterialName()
{
	return materialName_;
}
const std::vector<unsigned int> MeshSubset::getIndices()
{
	return indices_;
}