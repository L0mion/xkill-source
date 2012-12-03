#ifndef XKILL_RENDERER_MESHMODEL_H
#define XKILL_RENDERER_MESHMODEL_H

#include <vector>

#include "MeshGeometry.h"
#include "MeshMaterial.h"
#include "vertices.h"

class MeshModel
{
public:
	MeshModel();
	MeshModel(
		const MeshGeometry				geometry,
		const std::vector<MeshMaterial>	materials);
	~MeshModel();

	MeshGeometry getGeometry();
	
	unsigned int				getNumMaterials();
	std::vector<MeshMaterial>	getMaterials();
protected:
private:
	MeshGeometry				geometry_;
	std::vector<MeshMaterial>	materials_;
};

#endif //XKILL_RENDERER_MESHMODEL_H