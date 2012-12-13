#ifndef XKILL_UTILITIES_TEXDESCHEADER_H
#define XKILL_UTILITIES_TEXDESCHEADER_H

#include <string>

#include "dllUtilities.h"

//! Describes a header read from a .texdesc-file.
struct DLL_U TexDescHeader
{
	//! Does nothing.
	TexDescHeader(){}
	//! Initializes TexDescHeader with passed values.
	TexDescHeader(
		std::string headerName,
		std::string texPath)
	{
		headerName_	= headerName;
		texPath_	= texPath;
	}
	//! Does nothing.
	~TexDescHeader(){}

	std::string headerName_;	//!< Name of .texdesc.
	std::string texPath_;		//!< Path to collection of textures.
};

#endif //XKILL_IO_TEXDESCHEADER_H