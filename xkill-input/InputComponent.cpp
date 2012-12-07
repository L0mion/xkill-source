#include "InputComponent.h"

#include <xkill-utilities/EventManager.h>

#include "InputManager.h"
#include <iostream>

InputComponent::InputComponent()
{
	newDeviceSearchTimer_ = 0.0f;

	SUBSCRIBE_TO_EVENT(this, EVENT_RUMBLE);
	SUBSCRIBE_TO_EVENT(this, EVENT_MOUSE_MOVE);
	SUBSCRIBE_TO_EVENT(this, EVENT_KEY_PRESS);
	SUBSCRIBE_TO_EVENT(this, EVENT_KEY_RELEASE);
}

InputComponent::~InputComponent()
{
	delete inputManager_;
}

bool InputComponent::init(HWND windowHandle, std::vector<InputAttribute>* inputAttributes, std::string configFilePath, float searchTime)
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
	EventType type = e->getType();
	if(type == EVENT_RUMBLE)
	{
		handleRumbleEvent(static_cast<Event_Rumble*>(e));
	}
	if(type == EVENT_MOUSE_MOVE)
	{
		handleMouseMoveEvent(static_cast<Event_MouseMove*>(e));
	}
	if(type == EVENT_KEY_PRESS)
	{
		Event_KeyPress* ekp = static_cast<Event_KeyPress*>(e);
		handleKeyEvent(ekp->keyEnum, true);
	}
	if(type == EVENT_KEY_RELEASE)
	{
		Event_KeyRelease* ekr = static_cast<Event_KeyRelease*>(e);
		handleKeyEvent(ekr->keyEnum, false);
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

		if(device == nullptr)
			continue;

		InputDevice::InputState state = device->GetState();

		float moveSpeed = 30.0f;

		int nrAxes = state.axes.size();
		if(nrAxes >= 1)
			inputAttributes_->at(i).position.x = state.axes[0].GetValue() * moveSpeed;

		if(nrAxes >= 2)													    
			inputAttributes_->at(i).position.y = state.axes[1].GetValue() * moveSpeed;
																		    
		if(nrAxes >= 3)													    
			inputAttributes_->at(i).rotation.x = state.axes[2].GetValue() * delta;
																		    
		if(nrAxes >= 4)													    
			inputAttributes_->at(i).rotation.y = state.axes[3].GetValue() * delta;

		if(state.buttons.size() > 3)
		{
			if(state.buttons[1].isReleased())
			{
				Event_Rumble* er = new Event_Rumble(i, true, 100.0f, 1.0f, 1.0f);
				EventManager::getInstance()->sendEvent(er);
				delete er;
			}

			if(state.buttons[2].isReleased())
			{
				Event_Rumble* er = new Event_Rumble(i, false, 100.0f, 0.0f, 0.0f);
				EventManager::getInstance()->sendEvent(er);
				delete er;
			}

			//Projectile test
			if(state.buttons[0].isReleased())													   
				inputAttributes_->at(i).fire = true;

			if(state.buttons.size() > 7)
			{
				if(state.buttons[3].isDown())
					inputAttributes_->at(i).position.y = moveSpeed;
																		    
				if(state.buttons[4].isDown())
					inputAttributes_->at(i).position.x = -moveSpeed;

				if(state.buttons[5].isDown())
					inputAttributes_->at(i).position.y = -moveSpeed;

				if(state.buttons[6].isDown())
					inputAttributes_->at(i).position.x = moveSpeed;
			}
		}
		
		if(device->GetType() == device->QT_INPUT_DEVICE)
		{
			QTInputDevices* qtDevice = static_cast<QTInputDevices*>(device);

			qtDevice->setAxesToZero();
			qtDevice->updateButtons();
		}
	}
}

void InputComponent::handleRumbleEvent(Event_Rumble* e)
{
	InputDevice* device = inputManager_->GetDevice(e->deviceNr);

	if(device != nullptr)
	{
		if(e->runRumble)
			device->RunForceFeedback();
		else
			device->StopForceFeedback();

		device->SetForceFeedback(e->leftScale, e->rightScale);
	}
}

void InputComponent::handleMouseMoveEvent(Event_MouseMove* e)
{
	QTInputDevices* device = inputManager_->GetMouseAndKeyboard();

	// Test camera movement
	float x = 5.0f*(float)e->dx;
	float y = 5.0f*(float)e->dy;

	float mouseSensitivity = 0.1f;

	if(device != nullptr)
	{
		device->setAxis(2, x * mouseSensitivity);
		device->setAxis(3, y * mouseSensitivity);
	}
}

void InputComponent::handleKeyEvent(char key, bool pressed)
{
	QTInputDevices* device = inputManager_->GetMouseAndKeyboard();
		
	if(device != nullptr)
	{
		device->setButton(key, pressed);
	}
}
