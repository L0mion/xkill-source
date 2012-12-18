#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>

#include <fbxsdk.h>

class LoaderFbx
{
public:
	LoaderFbx();
	~LoaderFbx();

	void init(std::string filename);
private:


	FbxString getAttributeTypeName(FbxNodeAttribute::EType type);

	void printTabs();
	void printAttribute(FbxNodeAttribute* attribute);
	void printNode(FbxNode* node);

	bool loadVertices(FbxMesh* mesh);

	int numTabs_;

	FbxManager*		fbxManager_;
	FbxIOSettings*	fbxIOSettings_;
	FbxImporter*	fbxImporter_;
	FbxScene*		fbxScene_;
};

#endif