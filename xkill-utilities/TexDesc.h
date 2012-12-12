#ifndef XKILL_UTILITIES_TEXDESC_H
#define XKILL_UTILITIES_TEXDESC_H

#include <string>
#include <vector>

#include "dllUtilities.h"
#include "TexDescHeader.h"
#include "TexDescTex.h"

//! Describes a read .texdesc-file.
class DLL_U TexDesc
{
public:
	//! Initializes TexDesc with passed values.
	TexDesc(
		std::string				path,
		TexDescHeader			header,
		std::vector<TexDescTex> texDescs);
	//! Does nothing.
	~TexDesc();

	std::string					getPath();
	TexDescHeader&				getHeader();
	std::vector<TexDescTex>&	getTexDescs();
protected:
private:
	std::string				path_;		//!< Path to collection of textures.
	TexDescHeader			header_;	//!< Header of .texdesc.
	std::vector<TexDescTex>	texDescs_;	//!< Collection of texture descriptions.
};

#endif //XKILL_IO_TEXDESC_H