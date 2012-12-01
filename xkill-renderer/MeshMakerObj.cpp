#include <fstream>

#include "LoaderObj.h"
#include "LoaderMTL.h"
#include "vertices.h"
#include "MTLMaterial.h"
#include "SimpleStringSplitter.h"

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

	//loaderMTL_ not deleted here, as it is managed in loadMTL.
}

bool MeshMakerObj::init()
{
	bool sucessfulLoad = true;

	fileNamePGY_ = getFileNamePGY();

	if(!existingPGY(pathPGY_, fileNamePGY_))
	{
		sucessfulLoad = loadObj();
		if(sucessfulLoad)
			sucessfulLoad = loadMTLs();
		if(sucessfulLoad)
		{
			meshModel_ = makeMesh(loaderObj_->getObj());
			makePGY(meshModel_);
		}
	}
	else
	{
		//Pase pgy
	}

	return sucessfulLoad;
}

MeshModel* MeshMakerObj::getMesh()
{
	return meshModel_;
}

bool MeshMakerObj::loadObj()
{
	bool sucessfulLoad = true;

	loaderObj_ = new LoaderObj(pathObj_, fileNameObj_);
	sucessfulLoad = loaderObj_->init();

	return sucessfulLoad;
}
bool MeshMakerObj::existingPGY(std::string pathPGY, std::string fileNamePGY)
{
	std::string fullPathPGY = pathPGY + fileNamePGY; 

	std::ifstream ifile(fullPathPGY);
	return ifile.good();
}
MeshModel* MeshMakerObj::makeMesh(Obj obj)
{
	MeshGeometry<VertexPosNormTex> meshGeo = objGeoToMeshGeo(obj.getObjGeometry());
	return new MeshModel(meshGeo, materials_);
}
void MeshMakerObj::makePGY(MeshModel* model)
{
	std::ofstream pgy(
		fileNamePGY_, 
		std::ios::out | std::ios::binary);
	
	char fileType[4] = "pgy";
	PGYHeader header;
	for(unsigned int i = 0; i < 4; i++)
		header.fileType[i] = fileType[i];
	header.versionNum = 0.1f;

	pgy.write(reinterpret_cast<const char*>(&header), sizeof(PGYHeader));

	pgy.close();
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

	delete loaderMtl_; //clear memory

	return sucessfulLoad;
}
void MeshMakerObj::loadMTLMaterials(MTL mtl)
{
	std::vector<MTLMaterial> mtlMats = mtl.getMaterials();
	for(unsigned int i = 0; i < mtlMats.size(); i++)
		materials_.push_back(MTLToMeshMaterial(mtlMats[i]));
}

MeshMaterial MeshMakerObj::MTLToMeshMaterial(MTLMaterial mtl)
{
	DirectX::XMFLOAT3 ambientColor	= mtl.getAmbientColor();
	DirectX::XMFLOAT3 diffuseColor	= mtl.getDiffuseColor();
	DirectX::XMFLOAT3 specularColor	= mtl.getSpecularColor();
	DirectX::XMFLOAT3 reflectivity	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); //not defined in .obj?
	float specPow	= mtl.getSpecularPow();
	float alpha		= mtl.getAlpha();
	
	MeshMaterial meshMaterial(
		ambientColor,
		diffuseColor,
		specularColor,
		specPow,
		reflectivity,
		alpha != 0); //obs, float to bool convertion

	return meshMaterial;
}
MeshGeometry<VertexPosNormTex> MeshMakerObj::objGeoToMeshGeo(ObjGeometry<VertexPosNormTex> objGeo)
{
	std::vector<ObjGroup>			objGroups	= objGeo.getObjGroups();
	std::vector<VertexPosNormTex>	objVertices = objGeo.getVertices();

	std::vector<MeshSubset> meshSubsets;
	for(unsigned int i = 0; i < objGroups.size(); i++)
		meshSubsets.push_back(objGroupToMeshSubset(objGroups[i]));

	return MeshGeometry<VertexPosNormTex>(objVertices, meshSubsets); //format of vertices the same
}
MeshSubset MeshMakerObj::objGroupToMeshSubset(ObjGroup objGroup)
{
	std::string ssName					= objGroup.getName();
	std::string ssMaterial				= objGroup.getMaterial();
	std::vector<unsigned int> ssIndices	= objGroup.getIndices();

	return MeshSubset(ssName, ssMaterial, ssIndices);
}

std::string MeshMakerObj::getFileNamePGY()
{
	return fileNameObj_ + PGY;
}