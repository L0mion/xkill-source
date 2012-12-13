#ifndef XKILL_RENDERER_STRINGCONV_H
#define XKILL_RENDERER_STRINGCONV_H

#include <string>

//! Converts an unsigned int to an std::string.
std::string uintToString(const unsigned int uint);
//! Converts an std::string to a wchar_t-array.
wchar_t* stringToWstr(std::string string);

#endif //XKILL_RENDERER_STRINGCONV_H