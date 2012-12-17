#include <iostream>

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/TexDesc.h>

#include "Renderer.h"
#include "renderingUtilities.h"
#include "renderingComponent.h"

RenderingComponent::RenderingComponent(HWND windowHandle)
{
	//Subscribe to all the events RenderingComponent will need during execution.
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_POST_DESC_TEX);

	windowHandle_	= windowHandle;

	renderer_		= nullptr;
}
RenderingComponent::~RenderingComponent()
{
	SAFE_DELETE(renderer_);
}

void RenderingComponent::reset()
{
	SAFE_RESET(renderer_);
}
HRESULT RenderingComponent::init()
{
	HRESULT hr = S_OK;
	
	renderer_ = new Renderer(windowHandle_);
	hr = renderer_->init();

	return hr;
}

void RenderingComponent::onUpdate(float delta)
{
	renderer_->render(delta);
}
void RenderingComponent::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_RESIZE:
		event_WindowResize((Event_WindowResize*)e);
		break;
	case EVENT_POST_DESC_TEX:
		event_PostDescTex((Event_PostDescTex*)e);
		break;
	default:
		break;
	}
}

void RenderingComponent::event_WindowResize( Event_WindowResize* e )
{
	int width = e->width;
	int height = e->height;

	renderer_->resize(width, height);
}
void RenderingComponent::event_PostDescTex(Event_PostDescTex* e)
{
	TexDesc* texDesc = e->texDesc_;

	renderer_->loadTextures(texDesc);

	delete texDesc;
}

