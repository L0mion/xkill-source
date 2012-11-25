#ifndef XKILL_UTILITIES_DLLUTILITIES_H
#define XKILL_UTILITIES_DLLUTILITIES_H

/// Utility class used by everyone.
/** 
\defgroup utilities XKILL - Utilities 
*/

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_UTILITIES_DLLUTILITIES_H