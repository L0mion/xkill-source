#ifndef XKILL_RENDERER_MESHMAKER_H
#define XKILL_RENDERER_MESHMAKER_H

#include <Windows.h>
#include <fstream>
#include <string>

#include "MeshModel.h"
#include "MeshGeometry.h"
#include "MeshSubset.h"
#include "MeshMaterial.h"

#include "MTL.h"
#include "ObjGeometry.h"
#include "Obj.h"

class LoaderObj;
class LoaderMTL;

class MeshMakerObj
{
public:
	MeshMakerObj(
		const std::string pathObj,
		const std::string pathPGY,
		const std::string fileNameObj,
		const std::string pathMTL);
	~MeshMakerObj();

	bool init();

	MeshModel getMesh();
protected:
private:
	bool		loadObj();
	MeshModel	loadPGY();

	bool existingPGY(std::string pathPGY, std::string fileNamePGY);
	
	MeshModel	makeMesh(Obj obj);
	bool		makePGY(MeshModel model);
	
	bool loadMTLs();
	bool loadMTL(std::string fileNameMTL);
	void loadMTLMaterials(MTL mtl);
	
	MeshMaterial	MTLToMeshMaterial(MTLMaterial mtl);
	MeshGeometry	objGeoToMeshGeo(ObjGeometry objGeo);
	MeshSubset		objGroupToMeshSubset(ObjGroup objGroup);

	std::string getFileNamePGY();

	std::string pathObj_;
	std::string pathMTL_;
	std::string pathPGY_;
	std::string fileNameObj_;

	LoaderObj* loaderObj_;
	LoaderMTL* loaderMtl_;

	/*Intermediate*/
	std::vector<MeshMaterial> materials_;

	/*Result*/
	MeshModel meshModel_;
};

#endif //XKILL_RENDERER_MESHMAKER_H