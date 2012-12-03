#ifndef XKILL_UTILITIES_DLLUTILITIES_H
#define XKILL_UTILITIES_DLLUTILITIES_H

// Disable some warnings we know about but can't solve right now
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

/// Utility class used by everyone.
/** 
\defgroup UTILITIES XKILL - Utilities 
*/

#ifdef DLL_UTILITIES
#define DLL_U __declspec(dllexport)
#else
#define DLL_U __declspec(dllimport)
#endif //DLL_UTILITIES

#endif //XKILL_UTILITIES_DLLUTILITIES_H