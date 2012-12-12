#ifndef XKILL_UTILITIES_TEXDESCTEX_H
#define XKILL_UTILITIES_TEXDESCTEX_H

#include <string>

#include "dllUtilities.h"

struct DLL_U TexDescTex
{
	unsigned int	id_;
	std::string		fileName_;

	TexDescTex(
		unsigned int	id,
		std::string		fileName)
	{
		id_			= id;
		fileName_	= fileName;
	}
	TexDescTex() {}
};

#endif //XKILL_UTILITIES_TEXDESCTEX_H