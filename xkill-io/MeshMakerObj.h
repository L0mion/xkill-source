#ifndef XKILL_IO_MESHMAKER_H
#define XKILL_IO_MESHMAKER_H

#include <map>
#include <string>

#include <xkill-utilities/MeshModel.h>
#include <xkill-utilities/MeshGeometry.h>
#include <xkill-utilities/MeshSubset.h>
#include <xkill-utilities/MeshMaterial.h>

#include "MTL.h"
#include "ObjGeometry.h"
#include "Obj.h"
#include "VarStatus.h"

class LoaderObj;
class LoaderMTL;

//! Component responsible for loading a single .obj-file.
/*!
Note that this component also supports loading and writing to intermediate .pgy-format.
\ingroup xkill-mesh-io-obj
*/
class MeshMakerObj
{
public:
	//! Passes necessary parameters to MeshMakerObj and sets it to default state.
	/*!
	\param pathObj		Path up to .obj-file.
	\param pathPGY		Path up to .pgy-file.
	\param fileNameObj	Filename of .obj-file.
	\param pathMTL		Path up to .mtl-files.
	*/
	MeshMakerObj(
		const std::string pathObj,
		const std::string pathPGY,
		const std::string fileNameObj,
		const std::string pathMTL,
		std::map<std::string, unsigned int>* texNameToID);
	//! Deletes loaderObj_ and (OBS!) meshModel_.
	~MeshMakerObj();

	//! Loads mesh from .pgy-format if such exists. If not; the mesh is loaded from an .obj-file and then written to .pgy-format.
	bool init();

	MeshModel* claimMesh();
protected:
private:
	//! Initializes LoaderObj-object and proceeds to load .obj.
	bool loadObj();
	//! Initializes LoaderPgy-object and proceeds to load .pgy.
	bool loadPGY(WriteTimeUTC writeTimeUTC);

	//! Checks whether or not a corresponding .pgy-file exists.
	bool existingPGY(std::string pathPGY, std::string fileNamePGY);
	bool getLastWrittenToFile(std::string pathPGY, std::string fileNamePGY, WriteTimeUTC& out);
	
	//! Converts Obj-format loaded from .obj-file into general Mesh-type format.
	MeshModel* makeMesh(Obj obj);
	//! Writes loaded model into a .pgy-format.
	bool makePGY(MeshModel* model, WriteTimeUTC writeTimeUTC);
	
	//! Loads .mtl-files based on dependencies loaded from .obj.
	bool loadMaterials();
	//! Loads an .mtl-file.
	bool loadMTL(std::string fileNameMTL);
	//! Loads resulting materials from .mtl-files into general Mesh-type material format.
	void loadMTLMaterials(MTL mtl);
	
	//! Converts an .obj-type MTL-material into a general Mesh-type material format.
	const MeshMaterial	MTLToMeshMaterial(MTLMaterial mtl);
	//! Converts an .obj-type Geometry into a general Mesh-type Geometry format.
	const MeshGeometry	objGeoToMeshGeo(ObjGeometry objGeo);
	//! Convers an .obj-type group into a general Mesh-type Subset-format.
	const MeshSubset	objGroupToMeshSubset(ObjGroup objGroup);
	//! Searches through materials and converts the string-type identifier of Material into an index.
	const int			MTLNameToMaterialIndex(std::string mtlName);

	unsigned int getTexIDfromName(std::string texFilename);
	std::string getFileNamePGY();

	std::string pathObj_;		//!< Path up to .obj-file.
	std::string pathMTL_;		//!< Path up to .mtl-files.
	std::string pathPGY_;		//!< Path up to .pgy-file.
	std::string fileNameObj_;	//!< Filename of .obj-file.

	LoaderObj* loaderObj_;		//!< Used to load .obj-files.
	LoaderMTL* loaderMtl_;		//!< Used to load .mtl-files.

	std::map<std::string, unsigned int>* texNameToID_; //!< Borrowed variable used to translate texture file-names to IDs.

	/*Intermediate*/
	std::vector<MeshMaterial>	materials_;		//!< Generalized Mesh-type materials read from .mtl.
	std::vector<std::string>	materialID_;	//!< Used to locate index of material-type

	/*Result*/
	VarStatus<MeshModel>* meshModel_;	//!< Resulting Mesh read from file.
};

#endif //XKILL_IO_MESHMAKER_H