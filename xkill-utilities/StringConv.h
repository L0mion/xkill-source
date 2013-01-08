#ifndef XKILL_RENDERER_STRINGCONV_H
#define XKILL_RENDERER_STRINGCONV_H

#include <string>

#include "dllUtilities.h"

//! Converts an unsigned int to an std::string.
DLL_U std::string uintToString(const unsigned int uint);
//! Converts an std::string to a wchar_t-array.
DLL_U wchar_t* stringToWstr(std::string string);

#endif //XKILL_RENDERER_STRINGCONV_H