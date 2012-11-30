#ifndef XKILL_RENDERER_MESHMODEL_H
#define XKILL_RENDERER_MESHMODEL_H

#include <vector>

#include "MeshGeometry.h"
#include "MeshDependencies.h"

class MeshMaterial;
class MeshSubset;

struct VertexPosNormTex;

class MeshModel
{
public:
	MeshModel(
		MeshGeometry<VertexPosNormTex>*	geometry,
		MeshDependencies*				dependencies);
	~MeshModel();

	MeshGeometry<VertexPosNormTex>*	getGeometry();
	MeshDependencies*				getDependencies();
protected:
private:
	MeshGeometry<VertexPosNormTex>*	geometry_;
	MeshDependencies*				dependencies_;
};

#endif //XKILL_RENDERER_MESHMODEL_H