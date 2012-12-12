#ifndef XKILL_RENDERER_ANIMATEDMESH_H
#define XKILL_RENDERER_ANIMATEDMESH_H

#include <fstream>
#include <vector>
#include <map>
#include <string>

struct VertexPosNormTexTanSkinned;
struct M3DMaterial;
struct M3DSubset;

class SkinnedData;

class AnimatedMesh
{
public:
	AnimatedMesh();
	~AnimatedMesh();

	void init();

	std::vector<VertexPosNormTexTanSkinned>* getVertices()  const;
	std::vector<unsigned int>*				 getIndices()	const;
	std::vector<M3DSubset>*					 getSubsets()	const;
	std::vector<M3DMaterial>*				 getMaterials() const;
	SkinnedData*							 getSkinInfo()	const;

private:

	std::vector<VertexPosNormTexTanSkinned>* vertices_;
	std::vector<unsigned int>*				 indices_;
	std::vector<M3DSubset>*					 subsets_;
	std::vector<M3DMaterial>*				 materials_;
	SkinnedData*							 skinInfo_;
};

#endif //XKILL_RENDERER_ANIMATEDMESH_H