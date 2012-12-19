#ifndef XKILL_UTILITIES_TEXDESCTEX_H
#define XKILL_UTILITIES_TEXDESCTEX_H

#include <string>

#include "dllUtilities.h"

//! Description of texture to be loaded.
/*!
\ingroup UTILITIES
*/
struct DLL_U TexDescTex
{
	//! Initializes TexDescTex-type with passed values.
	TexDescTex(
		unsigned int	id,
		std::string		fileName)
	{
		id_			= id;
		fileName_	= fileName;
	}
	//! Does nothing.
	TexDescTex() {}

	unsigned int	id_;		//!< ID of texture.
	std::string		fileName_;	//!< Filename of texture.
};

#endif //XKILL_UTILITIES_TEXDESCTEX_H