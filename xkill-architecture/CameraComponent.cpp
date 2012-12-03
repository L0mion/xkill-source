#include "CameraComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/EventManager.h>
#include "Camera.h"

CameraComponent::CameraComponent()
{
	cameraAttributes_	= nullptr;
	cameraMovementAttribute_	= nullptr;
}

CameraComponent::~CameraComponent()
{
}
void CameraComponent::init()
{
	// Fetch attributes
	GET_ATTRIBUTES(cameraAttributes_, CameraAttribute, ATTRIBUTE_CAMERA);
	//GET_ATTRIBUTES(inputAttributes_, InputAttribute, ATTRIBUTE_INPUT);

	// fetch aspect ratio
	Event_getWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);
	float aspectRatio = windowResolution.getAspectRatio();

	if(cameraAttributes_->size()!=2)
	{
		for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
		{	
			cameras_.push_back(Camera(aspectRatio));
		}
	}
	else
	{
		aspectRatio /= 2;
		for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
		{	
			cameras_.push_back(Camera(aspectRatio));
		}
	}

	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		cameras_[i].updateProj();
		memcpy(cameraAttributes_->at(i).mat_projection, cameras_[i].getProjection().m, 16*4);
	}

	// listen to mouseMove events
	EventManager::getInstance()->addObserver(this, EVENT_MOUSE_MOVE);
}

void CameraComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_MOUSE_MOVE:
		event_MouseMove((Event_MouseMove*)e);
		break;
	default:
		break;
	}
}

void CameraComponent::onUpdate(float delta)
{
	for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
	{
		CameraAttribute* camera = &cameraAttributes_->at(i);
		SpatialAttribute* spatial = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, camera);
		PositionAttribute* position = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatial);
		cameras_[i].setPosition(position->position);
		cameras_[i].updateView();

		memcpy(cameraAttributes_->at(i).mat_view, cameras_[i].getView().m, 16*4);

		//cameras_[i].yaw(cameraMovementAttribute_->at(i).rotation[0]);
		//cameras_[i].pitch(cameraMovementAttribute_->at(i).rotation[1]);
		//cameras_[i].setPosition(cameraMovementAttribute_->at(i).position);
	}
}

void CameraComponent::event_MouseMove(Event_MouseMove* e)
{
	// Set 1 pixel = 0.25 degrees
	//float x = XMConvertToRadians(0.20f*(float)e->dx);
	//float y = XMConvertToRadians(0.20f*(float)e->dy);

	// Test camera movement
	float x = 5.0f*(float)e->dx;
	float y = 5.0f*(float)e->dy;
	cameras_[0].pitch(y);
	cameras_[0].yaw(x);
}