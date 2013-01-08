#include <sstream>

#include <xkill-utilities/EventManager.h>

#include "LoaderFbx.h"

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
	parseIndices(mesh, polygonVertexCount);
	
	int vertexId = 0;

	for(int polygonIndex=0; polygonIndex<polygonCount; polygonIndex++)
	{
		for(int insidePolygonIndex=0; insidePolygonIndex<POLYGON_SIZE; insidePolygonIndex++)
		{
			parseVertexNormals(mesh, polygonIndex, insidePolygonIndex, vertexId);
			parseVertexUVs(mesh, polygonIndex, insidePolygonIndex);		
			vertexId++;
		}
	}
}
void LoaderFbx::parseIndices(FbxMesh* mesh, int polygonVertexCount)
{
	int* indices = mesh->GetPolygonVertices();
	indices_.resize(polygonVertexCount);
	memcpy(&indices_[0], &indices[0], sizeof(int)*polygonVertexCount);
}
void LoaderFbx::parseVertexPositions(FbxMesh* mesh, int polygonVertexCount)
{
	FbxVector4* controlPoints = mesh->GetControlPoints();

	int* indices = mesh->GetPolygonVertices();
	for(int i=0; i<polygonVertexCount; i++)
	{
		Float3 position;
		position.x = controlPoints[indices[i]].mData[0];
		position.y = controlPoints[indices[i]].mData[1];
		position.z = controlPoints[indices[i]].mData[2];

		vertexPositions_.push_back(position);
	}
}
void LoaderFbx::parseVertexNormals(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int vertexId)
{
	FbxVector4 fbxNormal;
	mesh->GetPolygonVertexNormal(polygonIndex, insidePolygonIndex, fbxNormal);
	Float3 normal;
	normal.x = fbxNormal.mData[0];
	normal.y = fbxNormal.mData[1];
	normal.z = fbxNormal.mData[2];

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
		uv.x = fbxUV.mData[0];
		uv.y = fbxUV.mData[1];
		vertexUVs_.push_back(uv);
	}
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