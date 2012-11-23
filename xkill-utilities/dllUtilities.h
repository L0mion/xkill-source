#ifndef XKILL_UTILITIES_DLLUTILITIES_H
#define XKILL_UTILITIES_DLLUTILITIES_H

/*! \defgroup xkill-utilities xkill-utilities
	Utilities for all components of XKILL. */

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_UTILITIES_DLLUTILITIES_H