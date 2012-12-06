#include "CameraComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/EventManager.h>
#include "Camera.h"

CameraComponent::CameraComponent()
{
	cameraAttributes_	= nullptr;
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::init()
{
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);

	// fetch attributes
	GET_ATTRIBUTES(cameraAttributes_, CameraAttribute, ATTRIBUTE_CAMERA);
	GET_ATTRIBUTES(inputAttributes_, InputAttribute, ATTRIBUTE_INPUT);

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
		aspectRatio *= 2;
		for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
		{	
			cameras_.push_back(Camera(aspectRatio));
		}
	}

	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		cameras_[i].updateProj();
		cameraAttributes_->at(i).mat_projection.copy((float*)&cameras_[i].getProjection());
	}
}

void CameraComponent::onEvent(Event* e)
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_RESIZE:
		event_WindowResize((Event_WindowResize*)e);
		break;
	default:
		break;
	}
}

void CameraComponent::onUpdate(float delta)
{
	unsigned int nrOfCamerasWithInput = cameras_.size();
	if(nrOfCamerasWithInput > inputAttributes_->size())
		nrOfCamerasWithInput = inputAttributes_->size();

	for(unsigned int i = 0; i < nrOfCamerasWithInput; i++)
	{
		cameras_[i].yaw(inputAttributes_->at(i).rotation.x);
		cameras_[i].pitch(inputAttributes_->at(i).rotation.y);

		inputAttributes_->at(i).rotation.x = 0.0f;
		inputAttributes_->at(i).rotation.y = 0.0f;
	}

	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		CameraAttribute* camera = &cameraAttributes_->at(i);
		SpatialAttribute* spatial = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, camera);
		PositionAttribute* position = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatial);
		cameras_[i].setPosition((float*)&position->position);
		cameras_[i].updateView();
		
		cameraAttributes_->at(i).mat_view.copy((float*)&cameras_[i].getView());
	}
}

void CameraComponent::event_WindowResize(Event_WindowResize* e)
{
	float aspectRatio = e->getAspectRatio();

	// TODO: resize aspect ratios of all cameras 
}
