#include "ObjGroup.h"

ObjGroup::ObjGroup(const std::string mgName)
{
	this->name_	= mgName;
	materialName_		= "";
}
ObjGroup::~ObjGroup()
{
	//Do nothing.
}

void ObjGroup::pushIndex(unsigned int index)
{
	indices_.push_back(index);
}

void ObjGroup::setMaterialName(std::string mgMaterial)
{
	materialName_ = mgMaterial;
}

const std::string ObjGroup::getName()
{
	return name_;
}
const std::string ObjGroup::getMaterial()
{
	return materialName_;
}
const std::vector<unsigned int> ObjGroup::getIndices()
{
	return indices_;
}
const unsigned int ObjGroup::getNumIndices()
{
	return indices_.size();
}