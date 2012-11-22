#ifndef XKILL_SOUND_DLLSOUND_H
#define XKILL_SOUND_DLLSOUND_H

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_SOUND_DLLSOUND_H