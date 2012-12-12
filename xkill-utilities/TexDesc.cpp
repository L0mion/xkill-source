#include "TexDesc.h"

TexDesc::TexDesc(
	std::string				path,
	TexDescHeader			header,
	std::vector<TexDescTex> texDescs)
{
	path_		= path;
	header_		= header;
	texDescs_	= texDescs;
}
TexDesc::~TexDesc()
{
	//Do nothing.
}

std::string TexDesc::getPath()
{
	return path_;
}
TexDescHeader& TexDesc::getHeader()
{
	return header_;
}
std::vector<TexDescTex>& TexDesc::getTexDescs()
{
	return texDescs_;
}