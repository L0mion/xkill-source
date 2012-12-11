#include "TexDesc.h"

TexDesc::TexDesc(
	TexDescHeader header,
	std::vector<TexDescTex> texDescs)
{
	header_		= header;
	texDescs_	= texDescs;
}
TexDesc::~TexDesc()
{
	//Do nothing.
}

TexDescHeader& TexDesc::getHeader()
{
	return header_;
}
std::vector<TexDescTex>& TexDesc::getTexDescs()
{
	return texDescs_;
}