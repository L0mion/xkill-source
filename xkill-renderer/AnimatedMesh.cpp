#include <xkill-utilities/MeshVertices.h>

#include "AnimatedMesh.h"
#include "M3DMeshData.h"
#include "SkinnedData.h"

#include "renderingUtilities.h"

AnimatedMesh::AnimatedMesh()
{
	vertices_	= nullptr;
	indices_	= nullptr;
	subsets_	= nullptr;
	materials_	= nullptr;
	skinInfo_	= nullptr;
}

AnimatedMesh::~AnimatedMesh()
{
	SAFE_DELETE(vertices_);
	SAFE_DELETE(indices_);
	SAFE_DELETE(subsets_);
	SAFE_DELETE(materials_);
	SAFE_DELETE(skinInfo_);
}

void AnimatedMesh::init()
{
	vertices_	= new std::vector<VertexPosNormTexTanSkinned>();
	indices_	= new std::vector<unsigned int>();
	subsets_	= new std::vector<M3DSubset>();
	materials_	= new std::vector<M3DMaterial>();
	skinInfo_	= new SkinnedData();
}

std::vector<VertexPosNormTexTanSkinned>* AnimatedMesh::getVertices()	const
{
	return vertices_;
}
std::vector<unsigned int>*				 AnimatedMesh::getIndices()		const
{
	return indices_;
}
std::vector<M3DSubset>*					 AnimatedMesh::getSubsets()		const
{
	return subsets_;
}
std::vector<M3DMaterial>*				 AnimatedMesh::getMaterials()	const
{
	return materials_;
}
SkinnedData*							 AnimatedMesh::getSkinInfo()	const
{
	return skinInfo_;
}