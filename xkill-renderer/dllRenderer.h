#ifndef XKILL_RENDERER_DLLRENDERER_H
#define XKILL_RENDERER_DLLRENDERER_H

#ifdef DLL_RENDERER
#define DLL_R __declspec(dllexport)
#else
#define DLL_R __declspec(dllimport)
#endif //DLL_RENDERER

#endif //XKILL_RENDERER_DLLRENDERER_H