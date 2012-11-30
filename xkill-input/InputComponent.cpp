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
		inputManager_->FillAttribute(inputAttributes_->at(i), i, delta);
}