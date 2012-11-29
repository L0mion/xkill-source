#include "Obj.h"
#include "vertices.h"

Obj::Obj()
{
	//Do nothing.
}
Obj::Obj(
	const LPCWSTR moFilename,
	const ObjDependencies objDependencies,
	const MeshGeometry<VertexPosNormTex> moMeshGeometry)
{
	this->fileName_			= moFilename;
	this->objDependencies_	= objDependencies;
	this->meshGeometry_		= moMeshGeometry;
}
Obj::~Obj()
{
	//Do nothing.
}

LPCWSTR Obj::getFileName()
{
	return fileName_;
}
std::vector<std::string> Obj::getMTLs()
{
	return objDependencies_.getMTLFiles();
}
MeshGeometry<VertexPosNormTex> Obj::getMeshGeometry()
{
	return meshGeometry_;
}