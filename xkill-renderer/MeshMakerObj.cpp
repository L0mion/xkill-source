#include "LoaderObj.h"
#include "LoaderMTL.h"

#include "MeshMakerObj.h"

MeshMakerObj::MeshMakerObj(
	const LPCWSTR pathObj,
	const LPCWSTR fileNameObj)
{
	pathObj_		= pathObj;
	fileNameObj_	= fileNameObj;

	loaderObj_ = nullptr;
	loaderMtl_ = nullptr;
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

	if(!checkPGY())
	{
		sucessfulLoad = loadObj();
		if(sucessfulLoad)
			makeMesh(loaderObj_->getObj());
	}

	return sucessfulLoad;
}

bool MeshMakerObj::loadObj()
{
	bool sucessfulLoad = true;

	loaderObj_ = new LoaderObj(fileNameObj_, pathObj_);
	sucessfulLoad = loaderObj_->init();

	return sucessfulLoad;
}
bool MeshMakerObj::checkPGY()
{
	//ifstream my_file("test.txt");
	//if (my_file.good())
	//{
	//}

	return false; //tmep
}
void MeshMakerObj::makeMesh(const Obj obj)
{
	
}