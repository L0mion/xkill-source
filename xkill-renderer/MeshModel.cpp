#include "MeshMaterial.h"
#include "MeshSubset.h"
#include "vertices.h"

#include "MeshModel.h"

MeshModel::MeshModel(
	const MeshGeometry<VertexPosNormTex>	geometry,
	const std::vector<MeshMaterial>			materials)
{
	geometry_	= geometry;
	materials_	= materials;
}
MeshModel::~MeshModel()
{
}

MeshGeometry<VertexPosNormTex> MeshModel::getGeometry()
{
	return geometry_;
}
std::vector<MeshMaterial> MeshModel::getMaterials()
{
	return materials_;
}