#include "MeshMaterial.h"
#include "MeshSubset.h"
#include "vertices.h"

#include "MeshModel.h"

MeshModel::MeshModel(
	MeshGeometry<VertexPosNormTex>*	geometry,
	MeshDependencies*					dependencies)
{
	geometry_		= geometry;
	dependencies_	= dependencies;
}
MeshModel::~MeshModel()
{
	if(geometry_)
		delete geometry_;
	if(dependencies_)
		delete dependencies_;
}

MeshGeometry<VertexPosNormTex>* MeshModel::getGeometry()
{
	return geometry_;
}
MeshDependencies* MeshModel::getDependencies()
{
	return dependencies_;
}