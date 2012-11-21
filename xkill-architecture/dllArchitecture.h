#ifndef XKILL_ARCHITECTURE_DLLARCHITECTURE_H
#define XKILL_ARCHITECTURE_DLLARCHITECTURE_H

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_ARCHITECTURE_DLLARCHITECTURE_H