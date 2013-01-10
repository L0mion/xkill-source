#include <sstream>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderFbx.h"
#include "MaterialDescFbx.h"

LoaderFbx::LoaderFbx()
{
	fbxManager_ = nullptr;
	fbxScene_	= nullptr;
}
LoaderFbx::~LoaderFbx()
{
	fbxManager_->Destroy();
}

bool LoaderFbx::load(std::string filename)
{
	bool success = true;

	success = createFbxManager();
	if(success)
		success = createFbxScene();
	if(success)
		success = loadScene(filename);

	FbxNode* node = fbxScene_->GetRootNode();
	if(node)
	{
		for(int i=0; i<node->GetChildCount(); i++)
			parseNode(node->GetChild(i));
	}

	return success;
}

void LoaderFbx::createVerteciesPosNormTex()
{
	std::vector<VertexPosNormTex> vertices;
	vertices = assemblePosNormTex();

	indexPosNormTex(vertices);
}

bool LoaderFbx::createFbxManager()
{
	bool success = true;

	fbxManager_ = FbxManager::Create();
	if(!fbxManager_)
	{
		success = false;
		SHOW_MESSAGEBOX("LoaderFbx::createFbxManager | fbxManager_ could not be created!");
	}
	else
	{
		FbxIOSettings* fbxIOSettings = FbxIOSettings::Create(fbxManager_, IOSROOT);
		fbxManager_->SetIOSettings(fbxIOSettings);

		FbxString path = FbxGetApplicationDirectory();
		fbxManager_->LoadPluginsDirectory(path.Buffer());
	}

	return success;
}
bool LoaderFbx::createFbxScene()
{
	bool success = true;

	fbxScene_ = FbxScene::Create(fbxManager_, "Scene 1");
	if(!fbxScene_)
	{
		success = false;
		SHOW_MESSAGEBOX("LoaderFbx::createFbxScene | fbxScene_ could not be created");
	}
	return success;
}
bool LoaderFbx::loadScene(std::string filename)
{
	bool success = true;

	int fileMajor, fileMinor, fileRevision;
	int sdkMajor, sdkMinor, sdkRevision;
	std::stringstream message;

	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager_, "");
	success = fbxImporter->Initialize(filename.c_str(), -1, fbxManager_->GetIOSettings());
	fbxImporter->GetFileVersion(fileMajor, fileMinor, fileRevision);

	if(!success)
	{
		message.str("");
		message << "LoaderFbx::loadScene | Call to FbxImporter::Initialize() failed! \n Error returned: " 
				<< fbxImporter->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());

		if(fbxImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedYet ||
		   fbxImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedAnymore)
		{
			std::stringstream message;
			message.str("");
			message << "FBX file format version for this FBX SDK is " << sdkMajor << "." << sdkMinor << "." << sdkRevision << "\n"
					<< "FBX file format version for file " << filename << " is " << fileMajor << "." << fileMinor << "." << fileRevision;
			SHOW_MESSAGEBOX(message.str());
		}
	}
	else
	{
		success = fbxImporter->Import(fbxScene_);
	}

	if(!success)
	{
		message.str("");
		message << "LoaderFbx::loadScene | Call to FbxImporter::Import() failed! \n Error returned: "
				<< fbxImporter->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());
	}

	return success;
}

void LoaderFbx::parseNode(FbxNode* node)
{
	int errorCode = node->GetLastErrorID();
	if(errorCode != -1)
	{
		std::stringstream message;
		message << "LoaderFbx::parseNode | Error code : " << errorCode
				<< " Error message: " << node->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());
	}

	FbxNodeAttribute::EType attributeType;

	if(node->GetNodeAttribute() == NULL)
		printf("LoaderFbx::parseNode | NULL Node Attribute \n\n");
	else
	{
		attributeType = node->GetNodeAttribute()->GetAttributeType();

		switch(attributeType)
		{
		case FbxNodeAttribute::eMesh:
			parseMesh(node);
			break;
		case FbxNodeAttribute::eSkeleton:
			break;
		}
	}

	for(int i=0; i<node->GetChildCount(); i++)
		parseNode(node->GetChild(i));
}
void LoaderFbx::parseMesh(FbxNode* node)
{
	clearPreviousGeometryData();

	FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();

	int errorCode = mesh->GetLastErrorID();
	if(errorCode != -1)
	{
		std::stringstream message;
		message << "LoaderFbx::parseMesh | Error code : " << errorCode
				<< " Error message: " << mesh->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());
	}

	int polygonVertexCount = mesh->GetPolygonVertexCount();
	int polygonCount = polygonVertexCount / POLYGON_SIZE;
	
	parseVertexPositions(mesh, polygonVertexCount);
	
	int vertexId = 0;

	for(int polygonIndex=0; polygonIndex<polygonCount; polygonIndex++)
	{
		for(int insidePolygonIndex=0; insidePolygonIndex<POLYGON_SIZE; insidePolygonIndex++)
		{
			parseVertexNormals(mesh, polygonIndex, insidePolygonIndex, vertexId);
			parseVertexUVs(mesh, polygonIndex, insidePolygonIndex);
			parseVertexTangents(mesh, vertexId);
			parseVertexBinormals(mesh, vertexId);
			vertexId++;
		}
	}

	parseMaterial(mesh);
}
void LoaderFbx::parseVertexPositions(FbxMesh* mesh, int polygonVertexCount)
{
	FbxVector4* controlPoints = mesh->GetControlPoints();
	int* indices = mesh->GetPolygonVertices();

	for(int i=0; i<polygonVertexCount; i++)
	{
		Float3 position;
		position.x = static_cast<float>(controlPoints[indices[i]].mData[0]);
		position.y = static_cast<float>(controlPoints[indices[i]].mData[1]);
		position.z = static_cast<float>(controlPoints[indices[i]].mData[2]);

		vertexPositions_.push_back(position);
	}
}
void LoaderFbx::parseVertexNormals(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int vertexId)
{
	FbxVector4 fbxNormal;
	mesh->GetPolygonVertexNormal(polygonIndex, insidePolygonIndex, fbxNormal);
	Float3 normal;
	normal.x = static_cast<float>(fbxNormal.mData[0]);
	normal.y = static_cast<float>(fbxNormal.mData[1]);
	normal.z = static_cast<float>(fbxNormal.mData[2]);

	vertexNormals_.push_back(normal);
}
void LoaderFbx::parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex)
{
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);

	for(int i=0; i<uvSetNameList.GetCount(); i++)
	{
		const char* uvSetName = uvSetNameList.GetStringAt(i);

		FbxVector2 fbxUV; 
		mesh->GetPolygonVertexUV(polygonIndex, insidePolygonIndex, uvSetName, fbxUV);
		Float2 uv;
		uv.x = static_cast<float>(fbxUV.mData[0]);
		uv.y = static_cast<float>(fbxUV.mData[1]);
		vertexUVs_.push_back(uv);
	}
}
void LoaderFbx::parseVertexTangents(FbxMesh* mesh, int vertexId)
{
	for(int i=0; i<mesh->GetElementTangentCount(); i++)
	{
		FbxVector4 fbxTangent;

		FbxGeometryElementTangent* tangentElement = mesh->GetElementTangent(i);

		if( tangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int id = 0;
			switch(tangentElement->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				fbxTangent = tangentElement->GetDirectArray().GetAt(vertexId);
				break;
			case FbxGeometryElement::eIndexToDirect:
				id 			= tangentElement->GetIndexArray().GetAt(vertexId);
				fbxTangent	= tangentElement->GetDirectArray().GetAt(id);
				break;
			default:
				break;
			}
		}

		Float4 tangent;
		tangent.x = static_cast<float>(fbxTangent.mData[0]);
		tangent.y =	static_cast<float>(fbxTangent.mData[1]);
		tangent.z =	static_cast<float>(fbxTangent.mData[2]);
		tangent.w =	static_cast<float>(fbxTangent.mData[3]);
		vertexTangents_.push_back(tangent);
	}
}
void LoaderFbx::parseVertexBinormals(FbxMesh* mesh, int vertexId)
{
	for(int i=0; i<mesh->GetElementBinormalCount(); i++)
	{
		FbxVector4 fbxBinormal;
		FbxGeometryElementBinormal* binormalElement = mesh->GetElementBinormal(i);
		if(binormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int id=0;
			switch(binormalElement->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				fbxBinormal = binormalElement->GetDirectArray().GetAt(vertexId);
				break;
			case FbxGeometryElement::eIndexToDirect:
				id = binormalElement->GetIndexArray().GetAt(vertexId);
				fbxBinormal = binormalElement->GetDirectArray().GetAt(id);
				break;
			default:
				break;
			}
		}

		Float4 binormal;
		binormal.x = static_cast<float>(fbxBinormal.mData[0]);
		binormal.y = static_cast<float>(fbxBinormal.mData[1]);
		binormal.z = static_cast<float>(fbxBinormal.mData[2]);
		binormal.w = static_cast<float>(fbxBinormal.mData[3]);
		vertexBinormals_.push_back(binormal);
	}
}

void LoaderFbx::parseMaterial(FbxGeometry* geometry)
{
	MaterialDescFbx materialDesc;

	int numMaterials = 0;
	FbxNode* node = NULL;
	if(geometry)
	{
		node = geometry->GetNode();
		if(node)
			numMaterials = node->GetMaterialCount();
	}
	if(numMaterials > 0)
	{
		FbxColor					color;
		FbxPropertyT<FbxDouble3>	kFbxDouble3;
		FbxPropertyT<FbxDouble>		kFbxDouble;
		
		for(int materialIndex=0; materialIndex<numMaterials; materialIndex++)
		{
			FbxSurfaceMaterial* material = node->GetMaterial(materialIndex);
			
			FbxString name = material->GetName();
			printf("Material:  %s \n", name.Buffer());
			
			const FbxImplementation* implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_HLSL);
			FbxString implementationType = "HLHL";
			if(!implementation)
			{
				implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_CGFX);
				implementationType = "CGFX";
			}
			if(implementation)
				parseMaterialImplementation(material, implementation, materialIndex, &materialDesc);
			else if(material->GetClassId().Is(fbxsdk_2013_3::FbxSurfacePhong::ClassId))
				parseMaterialPhong(material, materialIndex, &materialDesc);
			else if(material->GetClassId().Is(FbxSurfaceLambert::ClassId))
				parseMaterialLambert(material, materialIndex, &materialDesc);
			else
				parseMaterialUnknown();
		}
	}
}
void LoaderFbx::parseMaterialImplementation(FbxSurfaceMaterial* material, const FbxImplementation* implementation, int materialIndex, MaterialDescFbx* materialDesc)
{
	FbxBindingTable const* rootTable = implementation->GetRootTable();
	FbxString fileName = rootTable->DescAbsoluteURL.Get();
	FbxString techniqueName = rootTable->DescTAG.Get();

	FbxBindingTable const* table = implementation->GetRootTable();
	size_t entryNum = table->GetEntryCount();

	for(int entryIndex=0; entryIndex<(int)entryNum; entryIndex++)
	{
		const FbxBindingTableEntry& entry = table->GetEntry(entryIndex);
		const char* entrySrcType = entry.GetEntryType(true);
		FbxProperty fbxProperty;

		FbxString test = entry.GetSource();
		printf("Entry: %s \n", test.Buffer());

		if(strcmp(FbxPropertyEntryView::sEntryType, entrySrcType) == 0)
		{
			fbxProperty = material->FindPropertyHierarchical(entry.GetSource());
			if(!fbxProperty.IsValid())
					fbxProperty = material->RootProperty.FindHierarchical(entry.GetSource());
		}
		else if(strcmp(FbxConstantEntryView::sEntryType, entrySrcType) == 0)
		{
			fbxProperty = implementation->GetConstants().FindHierarchical(entry.GetSource());
		}

		if(fbxProperty.IsValid())
		{
			if(fbxProperty.GetSrcObjectCount<FbxTexture>() > 0)
			{
				for(int i=0; i<fbxProperty.GetSrcObjectCount<FbxFileTexture>(); i++)
					materialDesc->addFbxFileTexture(fbxProperty.GetSrcObject<FbxFileTexture>(i));
				
				for(int i=0; i<fbxProperty.GetSrcObjectCount<FbxLayeredTexture>(); i++)
					materialDesc->addFbxLayeredTexture(fbxProperty.GetSrcObject<FbxLayeredTexture>(i));
				
				for(int i=0; i<fbxProperty.GetSrcObjectCount<FbxProceduralTexture>(); i++)
					materialDesc->addFbxProceduralTexture(fbxProperty.GetSrcObject<FbxProceduralTexture>(i));
			}
			else
			{
				FbxDataType fbxType = fbxProperty.GetPropertyDataType();
				
				/*************************************************************
				 *In the SDK example they extract various data types from the*
				 *property object here without saying what they mean and thus*
				 *I don't know what to do with it							 *
				 *************************************************************/ 
			}
		}
	}
}
void LoaderFbx::parseMaterialPhong(FbxSurfaceMaterial* material, int materialIndex, MaterialDescFbx* materialDesc)
{
	extractPhongAmbient(material, materialDesc);
	extractPhongDiffuse(material, materialDesc);
	extractPhongSpecular(material, materialDesc);
	extractPhongEmissive(material, materialDesc);
	extractPhongOpacity(material, materialDesc);
	extractPhongShininess(material, materialDesc);
	extractPhongReflectivity(material, materialDesc);
}
void LoaderFbx::parseMaterialLambert(FbxSurfaceMaterial* material, int materialIndex, MaterialDescFbx* materialDesc)
{
	extractLambertAmbient(material, materialDesc);
	extractLambertDiffuse(material, materialDesc);
	extractLambertEmissive(material, materialDesc);
	extractLambertOpacity(material, materialDesc);
}
void LoaderFbx::parseMaterialUnknown()
{
}

void LoaderFbx::extractPhongAmbient(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxAmbient;

	const char* debug = static_cast<FbxSurfacePhong*>(material)->sAmbient;
	printf("sAmbient: %s\n", debug); 

	fbxAmbient = static_cast<FbxSurfacePhong*>(material)->Ambient;
	Float3 ambient;
	ambient.x = static_cast<float>(fbxAmbient.Get()[0]);
	ambient.y = static_cast<float>(fbxAmbient.Get()[1]);
	ambient.z = static_cast<float>(fbxAmbient.Get()[2]);

	materialDesc->setAmbient(ambient);
}
void LoaderFbx::extractPhongDiffuse(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxDiffuse;
	fbxDiffuse = static_cast<FbxSurfacePhong*>(material)->Diffuse;
	Float3 diffuse;
	diffuse.x = static_cast<float>(fbxDiffuse.Get()[0]);
	diffuse.y = static_cast<float>(fbxDiffuse.Get()[1]);
	diffuse.z = static_cast<float>(fbxDiffuse.Get()[2]);
	materialDesc->setDiffuse(diffuse);
}
void LoaderFbx::extractPhongSpecular(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxSpecular;
	fbxSpecular = static_cast<FbxSurfacePhong*>(material)->Specular;
	Float3 specular;
	specular.x = static_cast<float>(fbxSpecular.Get()[0]);
	specular.y = static_cast<float>(fbxSpecular.Get()[1]);
	specular.z = static_cast<float>(fbxSpecular.Get()[2]);
	materialDesc->setSpecular(specular);
}
void LoaderFbx::extractPhongEmissive(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxEmissive;
	fbxEmissive = static_cast<FbxSurfacePhong*>(material)->Emissive;
	Float3 emissive;
	emissive.x = static_cast<float>(fbxEmissive.Get()[0]);
	emissive.y = static_cast<float>(fbxEmissive.Get()[1]);
	emissive.z = static_cast<float>(fbxEmissive.Get()[2]);
	materialDesc->setEmissive(emissive);
}
void LoaderFbx::extractPhongOpacity(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxOpacity;
	fbxOpacity = static_cast<FbxSurfacePhong*>(material)->TransparencyFactor;
	float opacity = static_cast<float>(fbxOpacity.Get());
	materialDesc->setOpacity(opacity);
}
void LoaderFbx::extractPhongShininess(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxShininess;
	fbxShininess = static_cast<FbxSurfacePhong*>(material)->Shininess;
	float shininess = static_cast<float>(fbxShininess.Get());
	materialDesc->setShininess(shininess);
}
void LoaderFbx::extractPhongReflectivity(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxReflectivity;
	fbxReflectivity = static_cast<FbxSurfacePhong*>(material)->ReflectionFactor;
	float reflectivity = static_cast<float>(fbxReflectivity.Get());
	materialDesc->setReflectivity(reflectivity);
}

void LoaderFbx::extractLambertAmbient(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxAmbient;
	fbxAmbient = static_cast<FbxSurfaceLambert*>(material)->Ambient;
	Float3 ambient;
	ambient.x = static_cast<float>(fbxAmbient.Get()[0]);
	ambient.y = static_cast<float>(fbxAmbient.Get()[1]);
	ambient.z = static_cast<float>(fbxAmbient.Get()[2]);
	materialDesc->setAmbient(ambient);
}
void LoaderFbx::extractLambertDiffuse(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxDiffuse;
	fbxDiffuse = static_cast<FbxSurfaceLambert*>(material)->Diffuse;
	Float3 diffuse;
	diffuse.x = static_cast<float>(fbxDiffuse.Get()[0]);
	diffuse.y = static_cast<float>(fbxDiffuse.Get()[1]);
	diffuse.z = static_cast<float>(fbxDiffuse.Get()[2]);
	materialDesc->setDiffuse(diffuse);
}
void LoaderFbx::extractLambertEmissive(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxEmissive;
	fbxEmissive = static_cast<FbxSurfaceLambert*>(material)->Emissive;
	Float3 emissive;
	emissive.x = static_cast<float>(fbxEmissive.Get()[0]);
	emissive.y = static_cast<float>(fbxEmissive.Get()[1]);
	emissive.z = static_cast<float>(fbxEmissive.Get()[2]);
	materialDesc->setEmissive(emissive);
}
void LoaderFbx::extractLambertOpacity(FbxSurfaceMaterial* material, MaterialDescFbx* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxOpacity;
	fbxOpacity = static_cast<FbxSurfaceLambert*>(material)->TransparencyFactor;
	float opacity = static_cast<float>(fbxOpacity.Get());
	materialDesc->setOpacity(opacity);
}

void LoaderFbx::parseAnimation(FbxScene* scene)
{
	for(int i=0; i<scene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(i);
	
		parseAnimationStack(animStack, scene->GetRootNode(), true);
		parseAnimationStack(animStack, scene->GetRootNode(), false);
	}
}
void LoaderFbx::parseAnimationStack(FbxAnimStack* animStack, FbxNode* node, bool isSwitcher)
{
	int numAnimationLayers = animStack->GetMemberCount<FbxAnimLayer>();
	
	for(int i=0; i<numAnimationLayers; i++)
	{
		FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);
		parseAnimationLayer(animLayer, node, isSwitcher);
	}
}
void LoaderFbx::parseAnimationLayer(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher)
{

}
void LoaderFbx::parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher)
{

}

void LoaderFbx::clearPreviousGeometryData()
{
	indices_.clear();
	vertexPositions_.clear();
	vertexNormals_.clear();
	vertexUVs_.clear();
	vertexTangents_.clear();
	vertexBinormals_.clear();
}

bool LoaderFbx::float2Equal(Float2 f1, Float2 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y)
		equal = true;

	return equal;
}
bool LoaderFbx::float3Equal(Float3 f1, Float3 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y && f1.z == f2.z)
		equal = true;

	return equal;
}
bool LoaderFbx::float4Equal(Float4 f1, Float4 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y && f1.z == f2.z && f1.w == f2.w)
		equal = true;

	return equal;
}

std::vector<VertexPosNormTex> LoaderFbx::assemblePosNormTex()
{
	std::vector<VertexPosNormTex> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosNormTex vertex;
		vertex.position_	= vertexPositions_[i];
		vertex.normal_		= vertexNormals_[i];
		vertex.texcoord_	= vertexUVs_[i];

		vertices.push_back(vertex);
	}
	return vertices;
}

void LoaderFbx::indexPosNormTex(std::vector<VertexPosNormTex> posNormTex)
{
	indices_.clear();
	verticesPosNormTex_.clear();

	for(unsigned int i=0; i<posNormTex.size(); i++)
	{
		bool equal	= false;
		int index	= 0;
		while(index<verticesPosNormTex_.size() && !equal)
		{
			equal = float3Equal(posNormTex[i].position_, verticesPosNormTex_[index].position_);
			if(equal)
				equal = float3Equal(posNormTex[i].normal_, verticesPosNormTex_[index].normal_);
			if(equal)
				equal = float2Equal(posNormTex[i].texcoord_, verticesPosNormTex_[index].texcoord_);
			if(!equal)
				index++;
		}
		if(equal)
			indices_.push_back(index);
		else
		{
			verticesPosNormTex_.push_back(posNormTex[i]);
			indices_.push_back(verticesPosNormTex_.size()-1);
		}
	}
}

std::vector<unsigned int> LoaderFbx::getIndices()
{
	return indices_;
}
std::vector<VertexPosNormTex> LoaderFbx::getVerticesPosNormTex()
{
	return verticesPosNormTex_;
}



//FbxVector4 fbxNormal;
//int id;
//
//for(int i=0; i<mesh->GetElementNormalCount(); i++)
//{
//	FbxGeometryElementNormal* elementNormal = mesh->GetElementNormal(i);
//
//	int errorCode = mesh->GetLastErrorID();
//	if(errorCode != -1)
//	{
//		std::stringstream message;
//		message << "LoaderFbx::parseMesh | Error code : " << errorCode
//				<< " Error message: " << mesh->GetLastErrorString();
//		SHOW_MESSAGEBOX(message.str());
//	}
//
//	FbxLayerElementArrayTemplate<FbxVector4> normals(elementNormal->GetDirectArray());
//	fbxNormal = elementNormal->GetDirectArray().GetAt(vertexId);
//
//	if(elementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//	{
//		switch(elementNormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//			fbxNormal = elementNormal->GetDirectArray().GetAt(vertexId);
//			break;
//		case FbxGeometryElement::eIndexToDirect:
//			id = elementNormal->GetIndexArray().GetAt(vertexId);
//			fbxNormal = elementNormal->GetDirectArray().GetAt(id);
//			break;
//		default:
//			break;
//		}
//	}
//	Float3 normal;
//	normal.x = fbxNormal.mData[0];
//	normal.y = fbxNormal.mData[1];
//	normal.z = fbxNormal.mData[2];
//	vertexNormals_.push_back(normal);
//}

//bool success = true;
//
//FbxArray<FbxVector4> normals;
//success = mesh->GetPolygonVertexNormals(normals);
//if(!success)
//{
//	std::stringstream message;
//	message << "LoaderFbx::parseVertexNormals | mesh->GetPolygonVertexNormals() failed \n "
//			<< "Error code: " << mesh->GetLastErrorID() << ". Error string: " << mesh->GetLastErrorString();
//	SHOW_MESSAGEBOX(message.str());
//}
//
//for(int i=0; i<normals.Size(); i++)
//{
//	Float3 normal;
//	normal.x = normals[i].mData[0];
//	normal.y = normals[i].mData[1];
//	normal.z = normals[i].mData[2];
//	
//	vertexNormals_.push_back(normal);
//}

//int* indices = mesh->GetPolygonVertices();
//indices_.resize(polygonVertexCount);
//memcpy(&indices_[0], &indices[0], sizeof(int)*polygonVertexCount);