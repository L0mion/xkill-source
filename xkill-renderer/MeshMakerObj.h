#ifndef XKILL_RENDERER_MESHMAKER_H
#define XKILL_RENDERER_MESHMAKER_H

#include <Windows.h>
#include <fstream>

#include "MeshModel.h"
#include "MeshGeometry.h"
#include "MeshSubset.h"
#include "MeshMaterial.h"

#include "MTL.h"
#include "ObjGeometry.h"

class LoaderObj;
class LoaderMTL;

class MeshMakerObj
{
public:
	MeshMakerObj(
		const LPCWSTR pathObj,
		const LPCWSTR fileNameObj,
		const LPCWSTR pathMTL);
	~MeshMakerObj();

	bool init();

	MeshModel* getMesh();
protected:
private:
	bool loadObj();
	bool parsePGY();
	void makeMesh(Obj obj);
	MeshGeometry<VertexPosNormTex> objGeoToMeshGeo(ObjGeometry<VertexPosNormTex> objGeo);
	MeshSubset objGroupToMeshSubset(ObjGroup objGroup);

	bool loadMTLs();
	bool loadMTL(std::string fileNameMTL);
	void loadMTLMaterials(MTL mtl);
	MeshMaterial MTLToMeshMaterial(MTLMaterial mtl);

	LPCWSTR pathObj_;
	LPCWSTR pathMTL_;
	LPCWSTR fileNameObj_;

	LoaderObj* loaderObj_;
	LoaderMTL* loaderMtl_;

	/*Intermediate*/
	std::vector<MeshMaterial> materials_;

	/*Result*/
	MeshModel* meshModel_;
};

#endif //XKILL_RENDERER_MESHMAKER_H