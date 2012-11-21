#ifndef XKILL_RENDERER_DLLRENDERER_H
#define XKILL_RENDERER_DLLRENDERER_H

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

#endif //XKILL_RENDERER_DLLRENDERER_H