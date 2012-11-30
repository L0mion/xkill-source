#include "LoaderObj.h"
#include "LoaderMTL.h"
#include "vertices.h"
#include "MTLMaterial.h"
#include "MeshMakerObj.h"

MeshMakerObj::MeshMakerObj(
	const LPCWSTR pathObj,
	const LPCWSTR fileNameObj,
	const LPCWSTR pathMTL)
{
	pathObj_		= pathObj;
	pathMTL_		= pathMTL;
	fileNameObj_	= fileNameObj;

	loaderObj_ = nullptr;
	loaderMtl_ = nullptr;

	meshModel_ = nullptr;
}
MeshMakerObj::~MeshMakerObj()
{
	if(loaderObj_)
		delete loaderObj_;
	if(loaderMtl_)
		delete loaderMtl_;
}

bool MeshMakerObj::init()
{
	bool sucessfulLoad = true;

	if(!parsePGY())
	{
		sucessfulLoad = loadObj();
		if(sucessfulLoad)
			sucessfulLoad = loadMTLs();
		if(sucessfulLoad)
			makeMesh(loaderObj_->getObj());
	}
	else
	{
		//Load binary
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
bool MeshMakerObj::parsePGY()
{
	return false; //tmep
}
void MeshMakerObj::makeMesh(Obj obj)
{
	MeshGeometry<VertexPosNormTex> meshGeo = objGeoToMeshGeo(obj.getObjGeometry());
	meshModel_ = new MeshModel(meshGeo, materials_);
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

	std::wstring stemp = std::wstring(fileNameMTL.begin(), fileNameMTL.end());
	LPCWSTR sw = stemp.c_str(); //get rid of this

	loaderMtl_ = new LoaderMTL(pathMTL_, sw);
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