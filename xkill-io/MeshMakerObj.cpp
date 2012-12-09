#include <fstream>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderObj.h"
#include "LoaderMTL.h"
#include "MTLMaterial.h"
#include "SimpleStringSplitter.h"
#include "WriterPGY.h"
#include "LoaderPGY.h"
#include "SpecsPGY.h"

#include "MeshMakerObj.h"

MeshMakerObj::MeshMakerObj(
	const std::string pathObj,
	const std::string pathPGY,
	const std::string fileNameObj,
	const std::string pathMTL)
{
	pathObj_		= pathObj;
	pathMTL_		= pathMTL;
	pathPGY_		= pathPGY;
	fileNameObj_	= fileNameObj;

	loaderObj_ = nullptr;
	loaderMtl_ = nullptr;

	meshModel_ = nullptr;
}
MeshMakerObj::~MeshMakerObj()
{
	if(loaderObj_)
		delete loaderObj_;

	if(meshModel_)
		delete meshModel_;

	//loaderMTL_ not deleted here, as it is managed in loadMTL.
}

bool MeshMakerObj::init()
{
	bool sucessfulLoad = true;

	std::string fileNamePGY = getFileNamePGY();

	if(!existingPGY(pathPGY_, fileNamePGY))
	{
		sucessfulLoad = loadObj();
		if(sucessfulLoad)
			sucessfulLoad = loadMTLs();
		if(sucessfulLoad)
		{
			meshModel_		= makeMesh(loaderObj_->getObj());
			sucessfulLoad	= makePGY(meshModel_);
		}
	}
	else
		meshModel_ = loadPGY();

	return sucessfulLoad;
}

MeshModel* MeshMakerObj::getMesh()
{
	return meshModel_;
}

bool MeshMakerObj::loadObj()
{
	bool sucessfulLoad = true;

	unsigned int flags = OBJ_PARSE_FLAGS_CHECK_NUMERIC | OBJ_PARSE_FLAGS_CHECK_NUM_PARAMS | OBJ_PARSE_FLAGS_IGNORE_EMPTY_GROUPS;
	loaderObj_ = new LoaderObj(pathObj_, fileNameObj_, flags);
	sucessfulLoad = loaderObj_->init();

	if(!sucessfulLoad)
		SHOW_MESSAGEBOX("Failed to load .obj-file: " + pathObj_ + fileNameObj_);

	return sucessfulLoad;
}
MeshModel* MeshMakerObj::loadPGY()
{
	MeshModel* loadedMesh = nullptr;
	bool sucessfulLoad = true;
	
	std::string fileNamePgy = getFileNamePGY();
	LoaderPGY pgyLoader(
		pathPGY_,
		fileNamePgy);
	sucessfulLoad = pgyLoader.init();

	if(sucessfulLoad)
		loadedMesh = pgyLoader.getMeshModel();

	return loadedMesh; //ugly, fix this
}

bool MeshMakerObj::existingPGY(std::string pathPGY, std::string fileNamePGY)
{
	std::string fullPathPGY = pathPGY + fileNamePGY; 

	std::ifstream ifile(fullPathPGY);
	return ifile.good();
}
MeshModel* MeshMakerObj::makeMesh(Obj obj)
{
	MeshGeometry meshGeo = objGeoToMeshGeo(obj.getObjGeometry());
	
	MeshModel* model = new MeshModel(meshGeo, materials_);
	return model;
}
bool MeshMakerObj::makePGY(MeshModel* model)
{
	std::string fileNamePgy = getFileNamePGY();
	WriterPGY pgyWriter(
		*meshModel_,
		pathPGY_,
		fileNamePgy);
	bool sucessfulWrite = pgyWriter.init();

	return sucessfulWrite;
}

bool MeshMakerObj::loadMTLs()
{
	bool sucessfulLoad = true;

	std::vector<std::string> mtls = loaderObj_->getObj().getMTLs();
	for(unsigned int i = 0; i < mtls.size() && sucessfulLoad; i++)
		sucessfulLoad = loadMTL(mtls[i]);

	return sucessfulLoad;
}
bool MeshMakerObj::loadMTL(std::string fileNameMTL)
{
	bool sucessfulLoad = true;

	loaderMtl_ = new LoaderMTL(pathMTL_, fileNameMTL);
	sucessfulLoad = loaderMtl_->init();

	if(sucessfulLoad)
		loadMTLMaterials(loaderMtl_->getMTL());
	else
		SHOW_MESSAGEBOX("Failed to load .mtl-file: " + pathMTL_ + fileNameMTL);

	delete loaderMtl_; //clear memory

	return sucessfulLoad;
}
void MeshMakerObj::loadMTLMaterials(MTL mtl)
{
	std::vector<MTLMaterial> mtlMats = mtl.getMaterials();
	for(unsigned int i = 0; i < mtlMats.size(); i++)
	{
		materials_.push_back(MTLToMeshMaterial(mtlMats[i]));
		materialID_.push_back(mtlMats[i].getName());
	}
}

const MeshMaterial MeshMakerObj::MTLToMeshMaterial(MTLMaterial mtl)
{
	Float3 ambientColor		= mtl.getAmbientColor();
	Float3 diffuseColor		= mtl.getDiffuseColor();
	Float3 specularColor	= mtl.getSpecularColor();
	Float3 reflectivity		= Float3(0.0f, 0.0f, 0.0f); //not defined in .obj?
	float specPow			= mtl.getSpecularPow();
	float alpha				= mtl.getAlpha();
	
	MeshMaterial meshMaterial(
		ambientColor,
		diffuseColor,
		specularColor,
		specPow,
		reflectivity,
		alpha != 0); //obs, float to bool convertion

	return meshMaterial;
}
const MeshGeometry MeshMakerObj::objGeoToMeshGeo(ObjGeometry objGeo)
{
	std::vector<ObjGroup>			objGroups	= objGeo.getObjGroups();
	std::vector<VertexPosNormTex>	objVertices = objGeo.getVertices();

	std::vector<MeshSubset> meshSubsets;
	for(unsigned int i = 0; i < objGroups.size(); i++)
		meshSubsets.push_back(objGroupToMeshSubset(objGroups[i]));

	return MeshGeometry(objVertices, meshSubsets); //format of vertices the same
}
const MeshSubset MeshMakerObj::objGroupToMeshSubset(ObjGroup objGroup)
{
	std::vector<unsigned int> indices	= objGroup.getIndices();
	int materialIndex = MTLNameToMaterialIndex(objGroup.getMaterial());

	return MeshSubset(materialIndex, indices);
}
const int MeshMakerObj::MTLNameToMaterialIndex(std::string mtlName)
{
	int materialIndex = 0;

	for(unsigned int i = 0; i < materialID_.size(); i++)
		if(materialID_[i] == mtlName)
			materialIndex = i;

	return materialIndex;
}

std::string MeshMakerObj::getFileNamePGY()
{
	return fileNameObj_ + PGY_SPECS_SUFFIX;
}