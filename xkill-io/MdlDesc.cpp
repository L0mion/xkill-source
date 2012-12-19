#include "MdlDesc.h"

MdlDesc::MdlDesc(
	std::string path,
	MdlDescHeader header,
	std::vector<MdlDescModel*> models)
{
	path_	= path;
	header_ = header;
	models_ = models;
}
MdlDesc::~MdlDesc()
{
	for(unsigned int i = 0; i < models_.size(); i++)
	{
		if(models_[i])
			delete models_[i];
	}
}

std::string MdlDesc::getPath() const
{
	return path_;
}
MdlDescHeader MdlDesc::getHeader() const
{
	return header_;
}
std::vector<MdlDescModel*> MdlDesc::getModels() const
{
	return models_;
}