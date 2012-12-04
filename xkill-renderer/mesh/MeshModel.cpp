#include "MeshMaterial.h"
#include "MeshSubset.h"
#include "vertices.h"

#include "MeshModel.h"

MeshModel::MeshModel()
{
	//Do nothing
}
MeshModel::MeshModel(
	const MeshGeometry				geometry,
	const std::vector<MeshMaterial>	materials)
{
	geometry_	= geometry;
	materials_	= materials;
}
MeshModel::~MeshModel()
{
}

MeshGeometry MeshModel::getGeometry()
{
	return geometry_;
}
unsigned int MeshModel::getNumMaterials()
{
	return materials_.size();
}
std::vector<MeshMaterial> MeshModel::getMaterials()
{
	return materials_;
}