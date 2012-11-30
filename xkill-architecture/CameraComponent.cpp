#include "CameraComponent.h"

#include <xkill-utilities/AttributeType.h>
#include "Camera.h"

CameraComponent::CameraComponent()
{
	cameraAttributes_	= nullptr;
	inputAttributes_	= nullptr;
}

CameraComponent::~CameraComponent()
{
	//if(cameraAttributes_)
	//	delete cameraAttributes_;
	//if(inputAttributes_)
	//	delete inputAttributes_;
}
void CameraComponent::init(std::vector<CameraAttribute>* cameraAttributes, std::vector<InputAttribute>* inputAttributes, float aspectRatio)
{
	cameraAttributes_ = cameraAttributes;
	inputAttributes_ = inputAttributes;
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
		CameraAttribute* camera = &cameraAttributes_->at(i);
		SpatialAttribute* spatial = ATTRIBUTE_CAST(SpatialAttribute, spatialAttribute, camera);
		PositionAttribute* position = ATTRIBUTE_CAST(PositionAttribute, positionAttribute, spatial);
		cameras_[i].setPosition(position->position);
		cameras_[i].updateView();

		memcpy(cameraAttributes_->at(i).mat_view, cameras_[i].getView().m, 16*4);

	//	cameras_[i].yaw(inputAttributes_->at(i).rotation[0]);
	//	cameras_[i].pitch(inputAttributes_->at(i).rotation[1]);
	//	cameras_[i].setPosition(inputAttributes_->at(i).position);
	}
}