#ifndef XKILL_UTILITIES_DLLUTILITIES_H
#define XKILL_UTILITIES_DLLUTILITIES_H

/// Utility class used by everyone.
/** 
\defgroup utilities XKILL - Utilities 
*/

#ifdef DLL_UTILITIES
#define DLL_U __declspec(dllexport)
#else
#define DLL_U __declspec(dllimport)
#endif //DLL_UTILITIES

#endif //XKILL_UTILITIES_DLLUTILITIES_H