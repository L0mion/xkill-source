#include <xkill-utilities/EventManager.h>

#include "LoaderFbx.h"

LoaderFbx::LoaderFbx()
{
	numTabs_		= 0;

	fbxManager_		= nullptr;
	fbxIOSettings_	= nullptr;
	fbxImporter_	= nullptr;
	fbxScene_		= nullptr;
}
LoaderFbx::~LoaderFbx()
{
	if(fbxIOSettings_)
		fbxIOSettings_->Destroy();
	if(fbxImporter_)
		fbxImporter_->Destroy();
	if(fbxScene_)
		fbxScene_->Destroy();
	if(fbxManager_)		//Manages memory and should be delete last.
		fbxManager_->Destroy();
}

bool LoaderFbx::load(std::string filename)
{
	bool succeeded = true;

	initFbxManager();
	initFbxIOSettings();

	succeeded = initFbxImporter(filename);

	initFbxScene();

	FbxNode* rootNode = fbxScene_->GetRootNode();
	if(rootNode)
	{
		int childCount = rootNode->GetChildCount();
		for(unsigned int i=0; i<rootNode->GetChildCount(); i++)
		{
			//printNode(rootNode->GetChild(i));
			parseNode(rootNode->GetChild(i));
		}
	}

	return succeeded;
}
void LoaderFbx::initFbxManager()
{
	fbxManager_ = FbxManager::Create();
	if(!fbxManager_)
		SHOW_MESSAGEBOX("LoaderFbx::initFbxManager | Initialization of fbxManager_ failed.");

	FbxString path = FbxGetApplicationDirectory();
	fbxManager_->LoadPluginsDirectory(path.Buffer());
}
void LoaderFbx::initFbxIOSettings()
{
	fbxIOSettings_ = FbxIOSettings::Create(fbxManager_, IOSROOT);
	fbxManager_->SetIOSettings(fbxIOSettings_);

	if(!fbxIOSettings_)
		SHOW_MESSAGEBOX("LoaderFbx::initFbxIOSettings | Initialization of fbxIOSettings failed.");

	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL,			true);
	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE,			true);
	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_LINK,				false);
	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE,			false);
	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO,				false);
	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION,		true);
	//fbxManager_->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS,	true);
}
bool LoaderFbx::initFbxImporter(std::string filename)
{
	bool succeeded = true;

	fbxImporter_ = FbxImporter::Create(fbxManager_, "");
	succeeded = fbxImporter_->Initialize(filename.c_str(), -1, fbxManager_->GetIOSettings());

	if(!succeeded)
		SHOW_MESSAGEBOX("LoaderFbx::initFbxImporter | Initialization of fbxImporter_ failed.");

	return succeeded;
}
void LoaderFbx::initFbxScene()
{
	fbxScene_ = FbxScene::Create(fbxManager_, "");
	if(!fbxScene_)
		SHOW_MESSAGEBOX("LoaderFbx::initFbxScene | Initialization of fbxScene_ failed.");
	fbxImporter_->Import(fbxScene_);
}

void	LoaderFbx::parseNode(FbxNode* node)
{
	if(node->GetLastErrorID() != -1)
	{
		int debug = node->GetLastErrorID();
		const char* errorStr = node->GetLastErrorString();
		int temp = 1;
	}

	const char* nodeName	= node->GetName();
	fbxDouble3 rotation		= node->LclRotation.Get();
	fbxDouble3 scaling		= node->LclScaling.Get();
	fbxDouble3 translation	= node->LclTranslation.Get();

	FbxMesh* mesh = (FbxMesh*)(node->GetNodeAttribute());
	switch(node->GetNodeAttribute()->GetAttributeType())
	{
	case FbxNodeAttribute::eSkeleton:
		break;
	case FbxNodeAttribute::eMesh:
		mesh = (FbxMesh*)(node->GetNodeAttribute());
		parseMesh(mesh);
		break;
	default:
		break;
	}
	
//	for(unsigned int i=0; i<node->GetChildCount(); i++)
//		parseNode(node->GetChild(i));

}
void	LoaderFbx::parseMesh(FbxMesh* mesh)
{
	int polygonCount = mesh->GetPolygonCount();

	FbxVector4* controlPoints = mesh->GetControlPoints();

	int vertexId = 0;

	for(int polygonIndex=0; polygonIndex<polygonCount; polygonIndex++)
	{
		parsePolyGroups(mesh, polygonIndex);

		int polygonSize = mesh->GetPolygonSize(polygonIndex);
		for(int insidePolygonIndex=0; insidePolygonIndex<polygonSize; insidePolygonIndex++)
		{
			int controlPointIndex = parseIndex(mesh, polygonIndex, insidePolygonIndex);

			parseVertexColor(mesh, controlPointIndex, vertexId);
			parseTexcoords(mesh, polygonIndex, insidePolygonIndex, controlPointIndex);

			vertexId++;
		}
	}
}
void	LoaderFbx::parsePolyGroups(FbxMesh* mesh, int polygonIndex)
{
	for(int polygonGroupIndex=0; polygonGroupIndex<mesh->GetElementPolygonGroupCount(); polygonGroupIndex++)
	{
		FbxGeometryElementPolygonGroup* polygonGroup = mesh->GetElementPolygonGroup(polygonGroupIndex);
		switch(polygonGroup->GetMappingMode())
		{
		case FbxGeometryElement::eByPolygon:
			if(polygonGroup->GetReferenceMode() == FbxGeometryElement::eIndex)
			{
				int polygonGroupId = polygonGroup->GetIndexArray().GetAt(polygonIndex);
				polygonGroups_.push_back(polygonGroupId);
			}
			break;
		default:
			SHOW_MESSAGEBOX("LoaderFbx | Unsupported group assignment");
			
		}
	}
}
int		LoaderFbx::parseIndex(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex)
{
	int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, insidePolygonIndex);
	indices_.push_back(controlPointIndex);

	return controlPointIndex;
}
void	LoaderFbx::parseVertexColor(FbxMesh* mesh, int controlPointIndex, int vertexId)
{
	int id;

	for(int i=0; i<mesh->GetElementVertexColorCount(); i++)
	{
		FbxGeometryElementVertexColor* vertexColor = mesh->GetElementVertexColor(i);
		switch(vertexColor->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch(vertexColor->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				vertexColors_.push_back(vertexColor->GetDirectArray().GetAt(controlPointIndex));
				break;
			case FbxGeometryElement::eIndexToDirect:
				id = vertexColor->GetIndexArray().GetAt(controlPointIndex);
				vertexColors_.push_back(vertexColor->GetDirectArray().GetAt(id));
				break;
			default:
				break;
			}
			break;
		case FbxGeometryElement::eByPolygonVertex:
			switch(vertexColor->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				vertexColors_.push_back(vertexColor->GetDirectArray().GetAt(vertexId));
				break;
			case FbxGeometryElement::eIndexToDirect:
				id = vertexColor->GetIndexArray().GetAt(vertexId);
				vertexColors_.push_back(vertexColor->GetDirectArray().GetAt(id));
				break;
			default:
				break;
			}
			break;

		case FbxGeometryElement::eByPolygon:
		case FbxGeometryElement::eAllSame:
		case FbxGeometryElement::eNone:
			break;
		}
	}
}
void	LoaderFbx::parseTexcoords(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int controlPointIndex)
{
	int id;

	int uvIndex;

	for(int i=0; i<mesh->GetElementUVCount()
		; i++)
	{
		FbxGeometryElementUV* uv = mesh->GetElementUV(i);

		switch(uv->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch(uv->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				texcoords_.push_back(uv->GetDirectArray().GetAt(controlPointIndex));
				break;
			case FbxGeometryElement::eIndexToDirect:
				id = uv->GetIndexArray().GetAt(controlPointIndex);
				texcoords_.push_back(uv->GetDirectArray().GetAt(id));
				break;
			default:
				break;
			}
			break;
		case FbxGeometryElement::eByPolygonVertex:
			uvIndex = mesh->GetTextureUVIndex(polygonIndex, insidePolygonIndex);
			switch(uv->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				texcoords_.push_back(uv->GetDirectArray().GetAt(uvIndex));
				break;
			case FbxGeometryElement::eIndexToDirect:
				texcoords_.push_back(uv->GetDirectArray().GetAt(uvIndex));
				break;
			default:
				break;
			}
			break;
		case FbxGeometryElement::eByPolygon:
			break;
		case FbxGeometryElement::eAllSame:  
			break;
		case FbxGeometryElement::eNone:     
			break;
		}
	}
}

FbxString LoaderFbx::getAttributeTypeName(FbxNodeAttribute::EType type)
{
	FbxString typeName = "uninitialized";

	switch(type)
	{
	case FbxNodeAttribute::eUnknown:
		typeName = "unknown";
		break;
	case FbxNodeAttribute::eNull:
		typeName = "null";
		break;
	case FbxNodeAttribute::eMarker:
		typeName = "marker";
	case FbxNodeAttribute::eSkeleton:
		typeName = "skeleton";
		break;
	case FbxNodeAttribute::eMesh:
		typeName = "mesh";
		break;
	case FbxNodeAttribute::eNurbs:
		typeName = "nurbs";
		break;
	case FbxNodeAttribute::ePatch:
		typeName = "patch";
		break;
	case FbxNodeAttribute::eCamera:
		typeName = "camera";
		break;
	case FbxNodeAttribute::eCameraStereo:
		typeName = "camera stereo";
		break;
	case FbxNodeAttribute::eCameraSwitcher:
		typeName = "camera switcher";
		break;
	case FbxNodeAttribute::eOpticalReference:
		typeName = "optical reference";
		break;
	case FbxNodeAttribute::eOpticalMarker:
		typeName = "optical marker";
		break;
	case FbxNodeAttribute::eNurbsCurve:
		typeName = "nurbs curve";
		break;
	case FbxNodeAttribute::eTrimNurbsSurface:
		typeName = "trim nurbs surface";
		break;
	case FbxNodeAttribute::eBoundary:
		typeName = "boundrary";
		break;
	case FbxNodeAttribute::eNurbsSurface:
		typeName = "nurbs surface";
		break;
	case FbxNodeAttribute::eShape:
		typeName = "shape";
		break;
	case FbxNodeAttribute::eLODGroup:
		typeName = "LOD group";
		break;
	case FbxNodeAttribute::eSubDiv:
		typeName = "subdiv";
		break;
	default:
		typeName = "uninitialized";
		break;
	}

	return typeName;
}
void LoaderFbx::printTabs()
{
	for(unsigned int i=0; i<numTabs_; i++)
		printf("\t");
}
void LoaderFbx::printAttribute(FbxNodeAttribute* attribute)
{
	if(attribute)
	{
		FbxString typeName		= getAttributeTypeName(attribute->GetAttributeType());
		FbxString attributeName = attribute->GetName();
		printTabs();

		printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attributeName.Buffer());
	}
}
void LoaderFbx::printNode(FbxNode* node)
{
	printTabs();
	const char* nodeName	= node->GetName();
	fbxDouble3 translation	= fbxDouble3(0.0f, 0.0f, 0.0f);//node->LclTranslation.Get();
	fbxDouble3 rotation		= node->LclRotation.Get();
	fbxDouble3 scaling		= node->LclScaling.Get();

	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
			nodeName,
			translation[0], translation[1], translation[2],
			rotation[0], rotation[1], rotation[2],
			scaling[0], scaling[1], scaling[2]);
	numTabs_++;

	for(unsigned int i=0; i<node->GetNodeAttributeCount(); i++)
		printAttribute(node->GetNodeAttributeByIndex(i));

	for(unsigned int i=0; i<node->GetChildCount(); i++)
		printNode(node->GetChild(i));

	numTabs_--;
	printTabs();
	printf("</node>\n");
}