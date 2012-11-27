#ifndef XKILL_SOUND_DLLSOUND_H
#define XKILL_SOUND_DLLSOUND_H

#ifdef DLL_SOUND
#define DLL_S __declspec(dllexport)
#else
#define DLL_S __declspec(dllimport)
#endif //DLL_SOUND

#endif //XKILL_SOUND_DLLSOUND_H