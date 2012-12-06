#ifndef XKILL_IO_DLLIO_H
#define XKILL_IO_DLLIO_H

#ifdef DLL_IO_FLAG
#define DLL_IO __declspec(dllexport)
#else
#define DLL_IO __declspec(dllimport)
#endif //DLL_IO

#endif //XKILL_IO_DLLIO_H