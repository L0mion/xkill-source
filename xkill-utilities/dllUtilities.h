#ifndef XKILL_UTILITIES_DLLUTILITIES_H
#define XKILL_UTILITIES_DLLUTILITIES_H

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_UTILITIES_DLLUTILITIES_H