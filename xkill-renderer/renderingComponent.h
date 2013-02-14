#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

#pragma warning(push)
#pragma warning(disable : 4005)
#define WINVER			0x0601
#define _WIN32_WINNT	0x0601
#pragma warning(pop)

#include "dllRenderer.h"
#include <xkill-utilities/IObserver.h>

typedef struct HWND__* HWND;
class Renderer;

/*! \defgroup xkill-renderer xkill-renderer
	Rendering Component of XKILL. */

//! Rendering Component of XKILL.
/*!
\ingroup xkill-renderer
*/
class DLL_R RenderingComponent : public IObserver
{
public:
	RenderingComponent(
		HWND windowHandle);
	~RenderingComponent();

	void reset();
	bool init();

	void onUpdate(float delta);	//!< Runs a frame for RenderingComponent.
	void onEvent(Event* e);		//!< Receives events for RenderingComponent.
private:
	HWND windowHandle_;
	Renderer* renderer_;
	 
	void event_WindowResize();
	void event_PostDescTex(Event_LoadTextures* e);
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H