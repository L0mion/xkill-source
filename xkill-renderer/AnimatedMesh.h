#ifndef XKILL_RENDERER_ANIMATEDMESH_H
#define XKILL_RENDERER_ANIMATEDMESH_H

#include <d3d11.h>

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

	HRESULT init(ID3D11Device* device);

	std::vector<VertexPosNormTexTanSkinned>* getVertices()  const;
	std::vector<unsigned int>*				 getIndices()	const;
	std::vector<M3DSubset>*					 getSubsets()	const;
	std::vector<M3DMaterial>*				 getMaterials() const;
	SkinnedData*							 getSkinInfo()	const;

	ID3D11Buffer* getVertexBuffer()	const;
	ID3D11Buffer* getIndexBuffer()	const;
								
	unsigned int getNumVertices()	const;
	unsigned int getNumIndices()	const;

private:

	HRESULT createVertexBuffer(ID3D11Device* device);
	HRESULT createIndexBuffer(ID3D11Device* device);

	std::vector<VertexPosNormTexTanSkinned>* vertices_;
	std::vector<unsigned int>*				 indices_;
	std::vector<M3DSubset>*					 subsets_;
	std::vector<M3DMaterial>*				 materials_;
	SkinnedData*							 skinInfo_;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;
};

#endif //XKILL_RENDERER_ANIMATEDMESH_H