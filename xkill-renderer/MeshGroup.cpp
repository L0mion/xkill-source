#include "MeshGroup.h"

MeshGroup::MeshGroup(const std::string mgName)
{
	this->mgName_	= mgName;
	mgMaterial_		= "";
}
MeshGroup::~MeshGroup()
{
	//Do nothing.
}

void MeshGroup::mgPushIndex(unsigned int index)
{
	mgIndices_.push_back(index);
}

bool MeshGroup::isValid()
{
	bool isValid = true;

	if(mgName_.size() <= 0)
		isValid = false;
	if(mgMaterial_.size() <= 0)
		isValid = false;
	if(mgIndices_.size() <= 0)
		isValid = false;

	return isValid;
}

void MeshGroup::setMGMaterial(std::string mgMaterial)
{
	mgMaterial_ = mgMaterial;
}

const std::string MeshGroup::getMGName()
{
	return mgName_;
}
const std::string MeshGroup::getMGMaterial()
{
	return mgMaterial_;
}
const std::vector<unsigned int> MeshGroup::getMGIndices()
{
	return mgIndices_;
}
const unsigned int MeshGroup::getMGNumIndices()
{
	return mgIndices_.size();
}