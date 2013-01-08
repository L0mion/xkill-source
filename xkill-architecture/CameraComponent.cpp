#include "CameraComponent.h"

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/EventManager.h>
#include "Camera.h"

CameraComponent::CameraComponent()
{
	// subscribe to events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);

	cameraAttributes_	= nullptr;
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::init()
{
	// fetch attributes
	cameraAttributes_ = GET_ATTRIBUTES(camera);
	inputAttributes_ = GET_ATTRIBUTES(input);


	// fetch aspect ratio
	Event_GetWindowResolution windowResolution;
	SEND_EVENT(&windowResolution);
	float aspectRatio = windowResolution.getAspectRatio();
	Attribute_Camera* cameraAttribute;
	if(cameraAttributes_->size()==2)
	{
		aspectRatio *= 2;
	}
	for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
	{	
		cameraAttribute = &cameraAttributes_->at(i);
		cameraAttribute->aspect = aspectRatio;
		cameras_.push_back(Camera(cameraAttribute->aspect,cameraAttribute->fov,cameraAttribute->zFar,cameraAttribute->zNear));
	}

	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		cameras_[i].updateProj();
		cameraAttributes_->at(i).mat_projection.copy((float*)&cameras_[i].getProjection());
	}

	return true;
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
	// Add extra camera
	while(cameraAttributes_->size() > cameras_.size())
	{
		Event_GetWindowResolution windowResolution;
		SEND_EVENT(&windowResolution);

		// Calculate split screen attribute
		float aspectRatio = windowResolution.getAspectRatio();

		// push new camera
		Attribute_Camera* cameraAttribute = &cameraAttributes_->at(cameras_.size());
		cameraAttribute->aspect = aspectRatio;
		cameras_.push_back(Camera(cameraAttribute->aspect,cameraAttribute->fov,cameraAttribute->zFar,cameraAttribute->zNear));
		if(cameraAttributes_->size()==2)
		{
			aspectRatio *= 2;
			for(unsigned int i=0; i<cameras_.size(); i++)
				cameras_[i].setAspectRatio(aspectRatio);
		}
	}
	for(unsigned int i=0; i<cameraAttributes_->size(); i++)
	{
		cameras_[i].updateProj();
		cameraAttributes_->at(i).mat_projection.copy((float*)&cameras_[i].getProjection());
	}

	// Remove unnecessary camera
	while(cameraAttributes_->size() < cameras_.size())
	{
		cameras_.pop_back();
		if(cameras_.size()==2)
		{
			for(unsigned i=0; i<cameras_.size(); i++)
			{
				Event_GetWindowResolution windowResolution;
				SEND_EVENT(&windowResolution);

				// Calculate split screen attribute
				float aspectRatio = windowResolution.getAspectRatio();
				if(cameraAttributes_->size()==2)
				{
					aspectRatio *= 2;
				}
				cameras_[i].setAspectRatio(aspectRatio);
				cameraAttributes_->at(i).aspect = aspectRatio;
			}
		}
	}

	// Update cameras
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
		Attribute_Camera* camera = &cameraAttributes_->at(i);
		Attribute_Spatial* spatial = ATTRIBUTE_CAST(Attribute_Spatial, ptr_spatial, camera);
		Attribute_Position* position = ATTRIBUTE_CAST(Attribute_Position, ptr_position, spatial);
		cameras_[i].setPosition((float*)&position->position);
		cameras_[i].updateView();
		
		cameraAttributes_->at(i).mat_view.copy((float*)&cameras_[i].getView());
	}
}

void CameraComponent::event_WindowResize(Event_WindowResize* e)
{
	float aspectRatio = e->getAspectRatio();

	for(unsigned int i=0; i<cameras_.size(); i++)
	{
		cameras_[i].setAspectRatio(aspectRatio);
		cameras_[i].updateProj();
	}
	// TODO: resize aspect ratios of all cameras 
}
