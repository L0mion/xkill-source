#ifndef XKILL_RENDERER_LOADERPGY_H
#define XKILL_RENDERER_LOADERPGY_H

#include "Loader.h"
#include "MeshModel.h"
#include "SpecsPGY.h"

class LoaderPGY : public Loader
{
public:
	LoaderPGY(
		const std::string filePath, 
		const std::string fileName);
	~LoaderPGY();

	bool init();

	const MeshModel getMeshModel();
protected:
private:
	const MeshModel						loadPGY();
	const PGYHeader						loadHeader();
	const std::vector<MeshMaterial>		loadMaterials(const unsigned int numMaterials);
	const MeshMaterial					loadMaterial();
	const MeshGeometry					loadGeometry(
		const unsigned int			numVertices,
		const unsigned int			numSubsets);
	const std::vector<VertexPosNormTex>	loadVertices(const unsigned int numVertices);
	const VertexPosNormTex				loadVertex();
	const std::vector<MeshSubset>		loadSubsets(const unsigned int numSubsets);
	const MeshSubset					loadSubset();

	MeshModel meshModel_;
};

#endif //XKILL_RENDERER_LOADERPGY_H