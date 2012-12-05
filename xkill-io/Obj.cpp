#include "Obj.h"
#include <xkill-utilities/MeshVertices.h>

Obj::Obj()
{
	//Do nothing.
}
Obj::Obj(
	const std::string		moFilename,
	const ObjDependencies	objDependencies,
	const ObjGeometry		moMeshGeometry)
{
	this->fileName_			= moFilename;
	this->objDependencies_	= objDependencies;
	this->objGeometry_		= moMeshGeometry;
}
Obj::~Obj()
{
	//Do nothing.
}

const std::string Obj::getFileName()
{
	return fileName_;
}
const std::vector<std::string> Obj::getMTLs()
{
	return objDependencies_.getMTLFiles();
}
const ObjGeometry Obj::getObjGeometry()
{
	return objGeometry_;
}