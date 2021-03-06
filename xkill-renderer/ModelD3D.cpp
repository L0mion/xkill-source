#include "VB.h"
#include "SubsetD3D.h"
#include "renderingUtilities.h"

#include "ModelD3D.h"



ModelD3D::ModelD3D(
	VertexType							vertexType,
	ID3D11Buffer*						vertexBuffer,
	const std::vector<SubsetD3D*>		subsets,
	const std::vector<MaterialDesc>		materials,
	SkinnedData*						skinnedData)
{
	vertexType_ = vertexType;
	vertexBuffer_	= vertexBuffer;
	subsets_		= subsets;
	materials_		= materials;
	skinnedData_	= skinnedData;

}
ModelD3D::~ModelD3D()
{
	SAFE_RELEASE(vertexBuffer_);

	for(unsigned int i = 0; i < subsets_.size(); i++)
	{
		if(subsets_[i])
			delete subsets_[i];
	}
//	SAFE_DELETE(skinnedData_);
}

ID3D11Buffer* ModelD3D::getVertexBuffer()
{
	return vertexBuffer_;
}
std::vector<SubsetD3D*>&	ModelD3D::getSubsetD3Ds()
{
	return subsets_;
}
std::vector<MaterialDesc>&	ModelD3D::getMaterials()
{
	return materials_;
}
VertexType ModelD3D::getVertexType()
{
	return vertexType_;
}
SkinnedData* ModelD3D::getSkinnedData()
{
	return skinnedData_;
}

bool ModelD3D::hasAnimation()
{
	bool animation = false;
	if(skinnedData_)
	{
		if(skinnedData_->getAnimations()->size() > 0)
			animation = true;
	}
	return animation;
}