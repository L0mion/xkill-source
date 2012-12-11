#ifndef XKILL_UTILITIES_TEXDESCHEADER_H
#define XKILL_UTILITIES_TEXDESCHEADER_H

#include <string>

#include "dllUtilities.h"

struct DLL_U TexDescHeader
{
	std::string headerName_;
	std::string texPath_;

	TexDescHeader(){}
	TexDescHeader(
		std::string headerName,
		std::string texPath)
	{
		headerName_	= headerName;
		texPath_	= texPath;
	}
	~TexDescHeader(){}
};

#endif //XKILL_IO_TEXDESCHEADER_H