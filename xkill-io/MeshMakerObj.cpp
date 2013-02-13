#include <windows.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/MeshVertices.h>
#include <xkill-utilities/StringConv.h>

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
	const std::string pathMTL,
	std::map<std::string, unsigned int>* texNameToID)
{
	pathObj_		= pathObj;
	pathMTL_		= pathMTL;
	pathPGY_		= pathPGY;
	fileNameObj_	= fileNameObj;

	loaderObj_ = nullptr;
	loaderMtl_ = nullptr;

	texNameToID_ = texNameToID;
}
MeshMakerObj::~MeshMakerObj()
{
	if(loaderObj_)
		delete loaderObj_;

	//loaderMTL_ not deleted here, as it is managed in loadMTL.
}

bool MeshMakerObj::init()
{
	bool sucessfulLoad = true;

	//Get time when original file was last edited.
	//WriteTimeUTC writeTimeUTC;
	//sucessfulLoad = getLastWrittenToFile(pathObj_, fileNameObj_, writeTimeUTC);
	//
	//std::string fileNamePGY = getFileNamePGY();
	//if(existingPGY(pathPGY_, fileNamePGY) && sucessfulLoad) //Attempt to load PGY
	//	sucessfulLoad = loadPGY(writeTimeUTC);
	//else
	//	sucessfulLoad = false;

	sucessfulLoad = loadObj();
	if(sucessfulLoad)
		sucessfulLoad = loadMaterials();
	if(sucessfulLoad)
	{
		Obj obj = loaderObj_->getObj();
		meshDesc_ = makeMesh(obj);
		//sucessfulLoad = makePGY(meshModel_, obj.getWriteTimeUTC());
	}
	return sucessfulLoad;
}

MeshDesc MeshMakerObj::getMesh()
{
	return meshDesc_;
}

bool MeshMakerObj::loadObj()
{
	bool sucessfulLoad = true;

	unsigned int flags = 
		OBJ_PARSE_FLAGS_CHECK_NUMERIC		| 
		OBJ_PARSE_FLAGS_CHECK_NUM_PARAMS	| 
		OBJ_PARSE_FLAGS_IGNORE_EMPTY_GROUPS;
	loaderObj_ = new LoaderObj(pathObj_, fileNameObj_, flags);
	sucessfulLoad = loaderObj_->init();

	if(!sucessfulLoad)
		SHOW_MESSAGEBOX("Failed to load .obj-file: " + pathObj_ + fileNameObj_);

	return sucessfulLoad;
}
//bool MeshMakerObj::loadPGY(WriteTimeUTC writeTimeUTC)
//{
//	MeshModel* loadedMesh = nullptr;
//	bool sucessfulLoad = true;
//	
//	std::string fileNamePgy = getFileNamePGY();
//	LoaderPGY pgyLoader(
//		pathPGY_,
//		fileNamePgy);
//	sucessfulLoad = pgyLoader.init();
//
//	if(sucessfulLoad)
//	{
//		if(writeTimeUTC == pgyLoader.getWriteTimeUTC())
//			meshDesc_->setVar(pgyLoader.getMeshModel());
//		else
//			sucessfulLoad = false; //Original file has been modified. Not valid.
//	}
//	
//	return sucessfulLoad;
//}

//bool MeshMakerObj::existingPGY(std::string pathPGY, std::string fileNamePGY)
//{
//	std::string fullPathPGY = pathPGY + fileNamePGY; 
//
//	std::ifstream ifile(fullPathPGY);
//	return ifile.good();
//}
bool MeshMakerObj::getLastWrittenToFile(std::string path, std::string fileName, WriteTimeUTC& out)
{
	WIN32_FIND_DATA findFileData;
	HANDLE searchHandleWinAPI;

#ifndef _UNICODE
	searchHandleWinAPI = FindFirstFile(
		fullPath.c_str(),
		&findFileData);
#else
	wchar_t* lpcwstr = stringToWstr(path + fileName);
	searchHandleWinAPI = FindFirstFile(
		lpcwstr,
		&findFileData);
	delete lpcwstr;
#endif //_UNICODE

	bool sucessfulRead = false;
	if(searchHandleWinAPI != INVALID_HANDLE_VALUE)
	{
		sucessfulRead = true;

		FILETIME lastWriteTime = findFileData.ftLastWriteTime;

		//Convert the last-write time to UTC-time.
		SYSTEMTIME utcTime;
		FileTimeToSystemTime(&lastWriteTime, &utcTime);

		out.year	= utcTime.wYear;
		out.month	= utcTime.wMonth;
		out.day		= utcTime.wDay;
		out.hour	= utcTime.wHour;
		out.min		= utcTime.wMinute;
		out.sec		= utcTime.wSecond;

		FindClose(searchHandleWinAPI);
	}

	return sucessfulRead;
}

MeshDesc MeshMakerObj::makeMesh(Obj obj)
{
	MeshDesc meshDesc;
	meshDesc.vertices_	= getVertexDescs(obj.getObjGeometry().getVertices());
	meshDesc.subsets_	= getSubsetDescs(obj.getObjGeometry().getObjGroups());
	meshDesc.materials_ = materials_;

	return meshDesc;
}
std::vector<VertexDesc> MeshMakerObj::getVertexDescs(std::vector<VertexPosNormTex> objVertices)
{
	std::vector<VertexDesc> vertexDescs(objVertices.size());
	for(unsigned int i = 0; i < objVertices.size(); i++)
	{
		VertexPosNormTex vertexObj = objVertices[i];
	
		VertexDesc vertexDesc;
		vertexDesc.position_			= vertexObj.position_;
		vertexDesc.normal_				= vertexObj.normal_;
		vertexDesc.textureCoordinates_	= vertexObj.texcoord_;

		vertexDescs[i] = vertexDesc;
	}

	return vertexDescs;
}
std::vector<SubsetDesc> MeshMakerObj::getSubsetDescs(std::vector<ObjGroup> objGroups)
{
	std::vector<SubsetDesc> subsetDescs(objGroups.size());
	for(unsigned int i = 0; i < objGroups.size(); i++)
	{
		ObjGroup objSubset = objGroups[i];

		SubsetDesc subsetDesc;
		subsetDesc.materialIndex_	= MTLNameToMaterialIndex(objSubset.getMaterial());
		subsetDesc.indices_			= objSubset.getIndices();

		subsetDescs[i] = subsetDesc;
	}

	return subsetDescs;
}

//bool MeshMakerObj::makePGY(MeshModel* model, WriteTimeUTC writeTimeUTC)
//{
//	std::string fileNamePgy = getFileNamePGY();
//	WriterPGY pgyWriter(
//		*model,
//		writeTimeUTC,
//		pathPGY_,
//		fileNamePgy);
//	bool sucessfulWrite = pgyWriter.init();
//
//	return sucessfulWrite;
//}

bool MeshMakerObj::loadMaterials()
{
	bool sucessfulLoad = true;

	std::vector<std::string> mtls = loaderObj_->getObj().getMTLs();
	if(mtls.size() > 0)
	{
		for(unsigned int i = 0; i < mtls.size() && sucessfulLoad; i++)
			sucessfulLoad = loadMTL(mtls[i]);
	}
	else
	{ //load default material
		MaterialDesc default;
		materials_.push_back(default);
		materialID_.push_back("default");
	}

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

const MaterialDesc MeshMakerObj::MTLToMeshMaterial(MTLMaterial mtl)
{
	Float3 ambientColor		= mtl.getAmbientColor();
	Float3 diffuseColor		= mtl.getDiffuseColor();
	Float3 specularColor	= mtl.getSpecularColor();
	Float3 reflectivity		= Float3(0.0f, 0.0f, 0.0f); //not defined in .obj?
	float specPow			= mtl.getSpecularPow();
	float alpha				= mtl.getAlpha();

	std::string texAlbedoName = mtl.getTexDiffuse();
	std::string texNormalName = mtl.getTexBump();
	
	MaterialDesc materialDesc;
	materialDesc.ambientTerm_	= ambientColor;
	materialDesc.diffuseTerm_	= diffuseColor;
	materialDesc.specularTerm_	= specularColor;
	materialDesc.reflectivity_	= reflectivity;
	materialDesc.specularPower_	= specPow;
	materialDesc.transperency_	= alpha != 0;
	materialDesc.idAlbedoTex_	= getTexIDfromName(texAlbedoName);
	materialDesc.idNormalTex_	= getTexIDfromName(texNormalName);

	return materialDesc;
}

const int MeshMakerObj::MTLNameToMaterialIndex(std::string mtlName)
{
	int materialIndex = 0;

	bool foundMat = false;
	for(unsigned int i = 0; i < materialID_.size() && !foundMat; i++)
	{
		if(materialID_[i] == mtlName)
		{
			materialIndex = i;
			foundMat = true;
		}
	}

	return materialIndex;
}

unsigned int MeshMakerObj::getTexIDfromName(std::string texFilename)
{
	unsigned int texID = 0;

	std::map<std::string, unsigned int>::iterator it;
	it = texNameToID_->find(texFilename);
	if(it != texNameToID_->end())
		texID = it->second;

	return texID;
}
//std::string MeshMakerObj::getFileNamePGY()
//{
//	return fileNameObj_ + PGY_SPECS_SUFFIX;
//}
