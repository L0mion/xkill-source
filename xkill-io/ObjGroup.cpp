#include "ObjGroup.h"

ObjGroup::ObjGroup(const std::string groupName)
{
	this->name_		= groupName;
	materialName_	= "";
}
ObjGroup::~ObjGroup()
{
	//Do nothing.
}

void ObjGroup::pushIndex(unsigned int index)
{
	indices_.push_back(index);
}

void ObjGroup::setMaterialName(std::string materialName)
{
	materialName_ = materialName;
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