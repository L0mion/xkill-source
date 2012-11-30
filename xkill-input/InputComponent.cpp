#include "InputComponent.h"

#include "InputManager.h"

InputComponent::InputComponent()
{

}

InputComponent::~InputComponent()
{
	delete inputManager_;
}

bool InputComponent::init(HWND windowHandle)
{
	windowHandle_ = windowHandle;

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
	inputManager_->Update(delta);
}

void InputComponent::handleEvents()
{

}