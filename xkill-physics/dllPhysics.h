#ifndef XKILL_PHYSICS_DLLPHYSICS_H
#define XKILL_PHYSICS_DLLPHYSICS_H

#ifdef DLL_EXPORT
  #define DLL __declspec(dllexport) 
#else
  #define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_PHYSICS_DLLPHYSICS_H