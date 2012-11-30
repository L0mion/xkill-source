#include "CameraComponent.h"

#include <xkill-utilities/AttributeType.h>
#include "Camera.h"

CameraComponent::CameraComponent()
{
	cameraAttributes_	= nullptr;
	cameraMovementAttribute_	= nullptr;
}

CameraComponent::~CameraComponent()
{
	//if(cameraAttributes_)
	//	delete cameraAttributes_;
	//if(cameraMovementAttribute_)
	//	delete cameraMovementAttribute_;
}
void CameraComponent::init(std::vector<CameraAttribute>* cameraAttributes, std::vector<CameraMovementAttribute>* cameraMovementAttribute, float aspectRatio)
{
	cameraAttributes_ = cameraAttributes;
	cameraMovementAttribute_ = cameraMovementAttribute;
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
}

void CameraComponent::onEvent(Event* e)
{

}

void CameraComponent::onUpdate(float delta)
{

	for(unsigned int i = 0; i < cameraAttributes_->size(); i++)
	{
		cameras_[i].updateView();

		memcpy(cameraAttributes_->at(i).mat_view, cameras_[i].getView().m, 16*4);

		//cameras_[i].yaw(cameraMovementAttribute_->at(i).rotation[0]);
		//cameras_[i].pitch(cameraMovementAttribute_->at(i).rotation[1]);
		//cameras_[i].setPosition(cameraMovementAttribute_->at(i).position);
	}
}