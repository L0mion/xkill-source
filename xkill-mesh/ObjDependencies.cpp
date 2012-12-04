#include "ObjDependencies.h"

ObjDependencies::ObjDependencies()
{
	//Do nothing.
}
ObjDependencies::~ObjDependencies()
{
	//Do nothing.
}

void ObjDependencies::pushDependencyMTL(const std::string mtlFile)
{
	mtlFiles_.push_back(mtlFile);
}

std::vector<std::string> ObjDependencies::getMTLFiles()
{
	return mtlFiles_;
}