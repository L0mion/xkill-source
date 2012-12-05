#include "InputComponent.h"

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-architecture/AttributeManager.h>

#include "InputManager.h"

InputComponent::InputComponent()
{
	newDeviceSearchTimer_ = 0.0f;

	ZeroMemory(&pos, sizeof(pos));

	EventManager::getInstance()->addObserver(this, EVENT_RUMBLE);
	EventManager::getInstance()->addObserver(this, EVENT_MOUSE_MOVE);
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
	if(e->getType() == EventType::EVENT_RUMBLE)
	{
		Event_Rumble* er = static_cast<Event_Rumble*>(e);

		InputDevice* device = inputManager_->GetDevice(er->deviceNr);

		if(device != nullptr)
		{
			if(er->runRumble)
				device->RunForceFeedback();
			else
				device->StopForceFeedback();

			device->SetForceFeedback(er->leftScale, er->rightScale);
		}
	}
	if(e->getType() == EventType::EVENT_MOUSE_MOVE)
	{
		Event_MouseMove* emm = static_cast<Event_MouseMove*>(e);

		// Set 1 pixel = 0.25 degrees
		//float x = XMConvertToRadians(0.20f*(float)e->dx);
		//float y = XMConvertToRadians(0.20f*(float)e->dy);

		// Test camera movement
		float x = 5.0f*(float)emm->dx;
		float y = 5.0f*(float)emm->dy;
		//cameras_[0].pitch(y);
		//cameras_[0].yaw(x);

		float mouseSensitivity = 0.001f;
		inputAttributes_->at(0).rotation.x += x * mouseSensitivity;
		inputAttributes_->at(0).rotation.y += y * mouseSensitivity;
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

		float moveSpeed = 3000.0f;

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
		}
	}
}