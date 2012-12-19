#include "VB.h"
#include "SubsetD3D.h"
#include "renderingUtilities.h"

#include "ModelD3D.h"

ModelD3D::ModelD3D(
	VB<VertexPosNormTex>*				vertexBuffer,
	const std::vector<SubsetD3D*>		subsets,
	const std::vector<MeshMaterial>		materials)
{
	vertexBuffer_	= vertexBuffer;
	subsets_		= subsets;
	materials_		= materials;
}
ModelD3D::~ModelD3D()
{
	if(vertexBuffer_)
		delete vertexBuffer_;

	for(unsigned int i = 0; i < subsets_.size(); i++)
	{
		if(subsets_[i])
			delete subsets_[i];
	}
}

VB<VertexPosNormTex>*		ModelD3D::getVertexBuffer()
{
	return vertexBuffer_;
}
std::vector<SubsetD3D*>&	ModelD3D::getSubsetD3Ds()
{
	return subsets_;
}
std::vector<MeshMaterial>&	ModelD3D::getMaterials()
{
	return materials_;
}