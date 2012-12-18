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

void LoaderFbx::init(std::string filename)
{
	fbxManager_ = FbxManager::Create();
	fbxIOSettings_ = FbxIOSettings::Create(fbxManager_, IOSROOT);
	fbxManager_->SetIOSettings(fbxIOSettings_);

	fbxImporter_ = FbxImporter::Create(fbxManager_, "");

	bool fileImported = true;
	fileImported = fbxImporter_->Initialize(filename.c_str(), -1, fbxManager_->GetIOSettings());

	if(!fileImported)
		SHOW_MESSAGEBOX("LoaderFbx::init | Could not initialize fbxImporter");

	fbxScene_ = FbxScene::Create(fbxManager_, "");
	fbxImporter_->Import(fbxScene_);

	FbxNode* rootNode = fbxScene_->GetRootNode();
	if(rootNode)
	{
		int childCount = rootNode->GetChildCount();
		for(unsigned int i=0; i<rootNode->GetChildCount(); i++)
			printNode(rootNode->GetChild(i));
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