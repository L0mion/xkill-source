#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif //DLL_EXPORT

class DLL RenderingComponent
{
private:
protected:
public:
	void youJustKissedABoomBoomBaby();

	RenderingComponent();
	~RenderingComponent();
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H