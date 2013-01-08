#include "Obj.h"
#include <xkill-utilities/MeshVertices.h>

Obj::Obj()
{
	//Do nothing.
}
Obj::Obj(
	const std::string		filename,
	const WriteTimeUTC		writeTimeUTC,
	const ObjDependencies	objDependencies,
	const ObjGeometry		objGeometry)
{
	this->fileName_			= filename;
	this->writeTimeUTC_		= writeTimeUTC;
	this->objDependencies_	= objDependencies;
	this->objGeometry_		= objGeometry;
}
Obj::~Obj()
{
	//Do nothing.
}

std::string Obj::getFileName() const
{
	return fileName_;
}
WriteTimeUTC Obj::getWriteTimeUTC() const
{
	return writeTimeUTC_;
}
const std::vector<std::string> Obj::getMTLs()
{
	return objDependencies_.getMTLFiles();
}
ObjGeometry Obj::getObjGeometry() const
{
	return objGeometry_;
}