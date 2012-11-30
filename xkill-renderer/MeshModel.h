#ifndef XKILL_RENDERER_MESHMODEL_H
#define XKILL_RENDERER_MESHMODEL_H

#include <vector>

#include "MeshGeometry.h"

class MeshMaterial;
class MeshSubset;

struct VertexPosNormTex;

class MeshModel
{
public:
	MeshModel(
		const MeshGeometry<VertexPosNormTex>	geometry,
		const std::vector<MeshMaterial>			materials);
	~MeshModel();

	MeshGeometry<VertexPosNormTex>	getGeometry();
	std::vector<MeshMaterial>			getMaterials();
protected:
private:
	MeshGeometry<VertexPosNormTex>	geometry_;
	std::vector<MeshMaterial>		materials_;
};

#endif //XKILL_RENDERER_MESHMODEL_H