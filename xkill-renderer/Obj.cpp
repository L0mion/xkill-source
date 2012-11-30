#include "Obj.h"
#include "vertices.h"

Obj::Obj()
{
	//Do nothing.
}
Obj::Obj(
	const LPCWSTR moFilename,
	const ObjDependencies objDependencies,
	const ObjGeometry<VertexPosNormTex> moMeshGeometry)
{
	this->fileName_			= moFilename;
	this->objDependencies_	= objDependencies;
	this->objGeometry_		= moMeshGeometry;
}
Obj::~Obj()
{
	//Do nothing.
}

const LPCWSTR Obj::getFileName()
{
	return fileName_;
}
const std::vector<std::string> Obj::getMTLs()
{
	return objDependencies_.getMTLFiles();
}
const ObjGeometry<VertexPosNormTex> Obj::getObjGeometry()
{
	return objGeometry_;
}