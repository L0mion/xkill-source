#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/Util.h>
#include <xkill-utilities/TexDesc.h>

#include "Renderer.h"
#include "renderingUtilities.h"
#include "renderingComponent.h"

RenderingComponent::RenderingComponent(HWND windowHandle)
{
	//Subscribe to all the events RenderingComponent will need during execution.
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_LOAD_TEXTURES);
	SUBSCRIBE_TO_EVENT(this, EVENT_GAMERESET);

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
bool RenderingComponent::init()
{
	HRESULT hr = S_OK;
	
	renderer_ = new Renderer(windowHandle_);
	hr = renderer_->init();

	return SUCCEEDED(hr);
}

void RenderingComponent::onUpdate(float delta)
{
	renderer_->update();
	renderer_->render();
}

void RenderingComponent::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_RESIZE:
		event_WindowResize();
		break;
	case EVENT_ATTRIBUTE_UPDATED:
		{
			// Reset if camera has been changed
			Event_AttributeUpdated* attributeUpdated = static_cast<Event_AttributeUpdated*>(e);
			if(attributeUpdated->attributeEnum == ATTRIBUTE_CAMERA)
			{
				event_WindowResize();
			}
		}
		break;
	case EVENT_GAMERESET:
		reset();
		break;
	case EVENT_LOAD_TEXTURES:
		event_PostDescTex((Event_LoadTextures*)e);
		break;
	default:
		break;
	}
}

void RenderingComponent::event_WindowResize()
{
	// Get new window size
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);
	int width = windowResolution.width;
	int height = windowResolution.height;

	// Resize renderer
	renderer_->resize(width, height);
}
void RenderingComponent::event_PostDescTex(Event_LoadTextures* e)
{
	TexDesc* texDesc = e->texDesc_;

	renderer_->loadTextures(texDesc);

	delete texDesc;
}