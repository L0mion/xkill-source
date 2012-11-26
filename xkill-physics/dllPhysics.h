#ifndef XKILL_PHYSICS_DLLPHYSICS_H
#define XKILL_PHYSICS_DLLPHYSICS_H

#ifdef DLL_PHYSICS
  #define DLL_P __declspec(dllexport) 
#else
  #define DLL_P __declspec(dllimport)
#endif //DLL_PHYSICS

#endif //XKILL_PHYSICS_DLLPHYSICS_H