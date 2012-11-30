#ifndef XKILL_INPUT_DLLINPUT_H
#define XKILL_INPUT_DLLINPUT_H

#ifdef DLL_INPUT
  #define DLL_I __declspec(dllexport) 
#else
  #define DLL_I __declspec(dllimport)
#endif //DLL_INPUT

#endif //XKILL_INPUT_DLLINPUT_H
