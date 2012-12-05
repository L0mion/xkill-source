#ifndef XKILL_IO_MESHMAKER_H
#define XKILL_IO_MESHMAKER_H

#include <string>

#include <xkill-utilities/MeshModel.h>
#include <xkill-utilities/MeshGeometry.h>
#include <xkill-utilities/MeshSubset.h>
#include <xkill-utilities/MeshMaterial.h>

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

	MeshModel* getMesh();
protected:
private:
	bool		loadObj();
	MeshModel*	loadPGY();

	bool existingPGY(std::string pathPGY, std::string fileNamePGY);
	
	MeshModel*	makeMesh(Obj obj);
	bool		makePGY(MeshModel* model);
	
	bool loadMTLs();
	bool loadMTL(std::string fileNameMTL);
	void loadMTLMaterials(MTL mtl);
	
	const MeshMaterial	MTLToMeshMaterial(MTLMaterial mtl);
	const MeshGeometry	objGeoToMeshGeo(ObjGeometry objGeo);
	const MeshSubset	objGroupToMeshSubset(ObjGroup objGroup);
	const int			MTLNameToMaterialIndex(std::string mtlName);

	std::string getFileNamePGY();

	std::string pathObj_;
	std::string pathMTL_;
	std::string pathPGY_;
	std::string fileNameObj_;

	LoaderObj* loaderObj_;
	LoaderMTL* loaderMtl_;

	/*Intermediate*/
	std::vector<MeshMaterial>	materials_;
	std::vector<std::string>	materialID_; //used to locate index of material-type

	/*Result*/
	MeshModel* meshModel_;
};

#endif //XKILL_IO_MESHMAKER_H