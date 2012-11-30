#include "InputComponent.h"

#include "InputManager.h"

InputComponent::InputComponent()
{
	newDeviceSearchTimer_ = 0.0f;

	ZeroMemory(&pos, sizeof(pos));
}

InputComponent::~InputComponent()
{
	delete inputManager_;
}

bool InputComponent::init(HWND windowHandle, std::vector<InputAttribute>* inputAttributes, float searchTime)
{
	inputAttributes_ = inputAttributes;

	windowHandle_ = windowHandle;
	searchTime_ = searchTime;

	inputManager_ = new InputManager();
	if(!inputManager_->InitInput(windowHandle))
		return false;

	return true;
}

void InputComponent::onEvent(Event* e)
{
	if(e->getType() == EventType::RUMBLE)
	{

	}
}

void InputComponent::onUpdate(float delta)
{
	newDeviceSearchTimer_ += delta;
	if(newDeviceSearchTimer_ >= searchTime_)
	{
		newDeviceSearchTimer_ = 0.0f;
		inputManager_->UpdateNumberOfGamepads(windowHandle_);
	}

	inputManager_->Update(delta);

	handleInput(delta);
}

void InputComponent::handleInput(float delta)
{
	for(unsigned int i = 0; i < inputAttributes_->size(); i++)
	{
		InputDevice* device = inputManager_->GetDevice(i);

		if(device == NULL)
			continue;

		InputDevice::InputState state =device->GetState();

		int nrAxes = state.axes.size();
		if(nrAxes >= 1)
			inputAttributes_->at(i).position[0] += state.axes[0].GetValue();// * delta;
																		   
		if(nrAxes >= 2)													   
			inputAttributes_->at(i).position[1] += state.axes[1].GetValue();// * delta;
																		   
		if(nrAxes >= 3)													   
			inputAttributes_->at(i).rotation[0] += state.axes[2].GetValue();// * delta;
																		   
		if(nrAxes >= 4)													   
			inputAttributes_->at(i).rotation[1] += state.axes[3].GetValue();// * delta;
	}
}