#ifndef XKILL_RENDERER_RENDERINGCOMPONENT_H
#define XKILL_RENDERER_RENDERINGCOMPONENT_H

#define WINVER			0x0601
#define _WIN32_WINNT	0x0601

typedef struct HWND__* HWND;

class Renderer;

#include <stdio.h>
#include <d3d11.h>
#include <vector>

#include <xkill-utilities/IObserver.h>

#include "dllRenderer.h"

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
	HRESULT init();

	void onUpdate(float delta);	//!< Runs a frame for RenderingComponent.
	void onEvent(Event* e);		//!< Receives events for RenderingComponent.
private:
	HWND windowHandle_;
	Renderer* renderer_;
	 
	void event_WindowResize(Event_WindowResize* e);
	void event_PostDescTex(Event_LoadTextures* e);
	//void event_CreateLight(Event_CreateLight* e);
};

#endif //XKILL_RENDERER_RENDERINGCOMPONENT_H