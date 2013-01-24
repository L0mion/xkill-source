#include "InputComponent.h"

#include <xkill-utilities/EventManager.h>
#include "InputManager.h"

ATTRIBUTES_DECLARE_ALL;

InputComponent::InputComponent()
{
	ATTRIBUTES_INIT_ALL;

	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_RUMBLE);
	SUBSCRIBE_TO_EVENT(this, EVENT_MOUSE_MOVE);
	SUBSCRIBE_TO_EVENT(this, EVENT_KEY_PRESS);
	SUBSCRIBE_TO_EVENT(this, EVENT_MOUSE_PRESS);

	inputManager_ = nullptr;
	newDeviceSearchTimer_ = 0.0f;
}

InputComponent::~InputComponent()
{
	SAFE_DELETE(inputManager_);
}

bool InputComponent::init(HWND windowHandle, std::string configFilePath, float searchTime)
{
	windowHandle_ = windowHandle;
	searchTime_ = searchTime;

	inputManager_ = new InputManager();
	if(!inputManager_->InitInput(windowHandle, configFilePath))
		return false;

	setupPlayerControllerConnection();

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
		int keyEnum	= ekp->keyEnum;
		bool isPressed = ekp->isPressed;

		handleKeyEvent(keyEnum, isPressed);
	}
	if(type == EVENT_MOUSE_PRESS)
	{
		Event_MousePress* emp = static_cast<Event_MousePress*>(e);
		int keyEnum	= emp->keyEnum;
		bool isPressed = emp->isPressed;

		handleMousePressedEvent(keyEnum, isPressed);
	}
	if(type == EVENT_INPUT_DEVICE_SEARCH)
	{
		inputManager_->UpdateNumberOfGamepads(windowHandle_);
	}
	if(type == EVENT_START_DEATHMATCH)
	{
		setupPlayerControllerConnection();
	}
}

void InputComponent::onUpdate(float delta)
{
	inputManager_->Update(delta);

	handleInput(delta);
}

void InputComponent::handleInput(float delta)
{
	while(itrPlayer.hasNext())
	{
		Attribute_Player* player = itrPlayer.getNext();

		AttributePointer ptr = player->ptr_inputDevice;

		if(ptr.host == nullptr)
			continue;

		InputDevice* device = itrInputDevice.at(ptr)->device;
		Attribute_Input* input = itrInput.at(player->ptr_input);

		if(device == nullptr)
			continue;

		input->position.x = device->getFloatValue(InputAction::ACTION_F_WALK_LR, delta);
		input->position.y = device->getFloatValue(InputAction::ACTION_F_WALK_FB, delta);
		input->rotation.x = device->getFloatValue(InputAction::ACTION_F_LOOK_LR, delta, true);
		input->rotation.y = device->getFloatValue(InputAction::ACTION_F_LOOK_UD, delta, true);

		if(device->getBoolValue(InputAction::ACTION_B_FIRE))
			input->fire = true;
		if(device->getBoolReleased(InputAction::ACTION_B_CHANGE_AMMUNITIONTYPE))
			input->changeAmmunitionType = true;
		if(device->getBoolReleased(InputAction::ACTION_B_CHANGE_FIRINGMODE))
			input->changeFiringMode = true;

		if(device->getBoolReleased(InputAction::ACTION_B_TOGGLE_MUTE_SOUND))
			SEND_EVENT(&Event_PlaySound(-1, true));

		if(device->getBoolReleased(InputAction::ACTION_B_RUMBLE_ON))
		{
			Event_Rumble* er = new Event_Rumble(itrPlayer.ownerId(), true, 100.0f, 1.0f, 1.0f);
			EventManager::getInstance()->sendEvent(er);
			delete er;
		}

		if(device->getBoolReleased(InputAction::ACTION_B_RUMBLE_OFF))
		{
			Event_Rumble* er = new Event_Rumble(itrPlayer.ownerId(), false, 100.0f, 0.0f, 0.0f);
			EventManager::getInstance()->sendEvent(er);
			delete er;
		}

		if(device->getBoolValue(InputAction::ACTION_B_WALK_FORWARD))
			input->position.y = 1.0f;
															
		if(device->getBoolValue(InputAction::ACTION_B_WALK_LEFT))
			input->position.x = -1.0f;
		
		if(device->getBoolValue(InputAction::ACTION_B_WALK_BACKWARD))
			input->position.y = -1.0f;
		
		if(device->getBoolValue(InputAction::ACTION_B_WALK_RIGHT))
			input->position.x = 1.0f;

		if(device->getBoolValue(InputAction::ACTION_B_LOOK_UP))
			input->rotation.y = -1.0f * delta;
															
		if(device->getBoolValue(InputAction::ACTION_B_LOOK_LEFT))
			input->rotation.x = -1.0f * delta;
		
		if(device->getBoolValue(InputAction::ACTION_B_LOOK_DOWN))
			input->rotation.y = 1.0f * delta;
		
		if(device->getBoolValue(InputAction::ACTION_B_LOOK_RIGHT))
			input->rotation.x = 1.0f * delta;
		
		if(device->getBoolReleased(InputAction::ACTION_B_KILL_PLAYER))
			input->killPlayer = true;

		if(device->getBoolValue(InputAction::ACTION_B_JUMP))
			input->jump = true;

		if(device->getBoolValue(InputAction::ACTION_B_SPRINT))
			input->sprint = true;

		float x, y;

		x = input->position.x;
		y = input->position.y;

		float length = std::sqrt(x*x + y*y);

		if(length > 1.0f) // The character shouldn't move faster than set speed
		{
			x = x/length;
			y = y/length;

			input->position.x = x;
			input->position.y = y;
		}

		if(device->GetType() == device->QT_INPUT_DEVICE) // Need to update QT devices since they're event based
		{
			QTInputDevices* qtDevice = static_cast<QTInputDevices*>(device);

			qtDevice->setAxesToZero();
			qtDevice->updateButtons();
		}
	}
}

void InputComponent::setupPlayerControllerConnection()
{
	while(itrPlayer.hasNext())
	{
		Attribute_Player* player = itrPlayer.getNext();

		if(itrInputDevice.hasNext())
		{
			player->ptr_inputDevice = itrInputDevice.attributePointer(itrInputDevice.getNext());
		}
		else
		{
			DEBUGPRINT("Player(s) could not be matched to a controller");
			break;
		}
	}

	itrInputDevice.resetIndex();
	itrPlayer.resetIndex();
}

void InputComponent::handleRumbleEvent(Event_Rumble* e)
{
 	InputDevice* device = inputManager_->GetDevice(e->deviceNr);

	if(device != nullptr)
	{
		if(e->runRumble)
			device->RunForceFeedback(e->duration);
		else
			device->StopForceFeedback();

		device->SetForceFeedback(e->leftScale, e->rightScale);
	}
}

void InputComponent::handleMouseMoveEvent(Event_MouseMove* e)
{
	QTInputDevices* device = inputManager_->GetMouseAndKeyboard();

	if(device != nullptr)
	{
		device->setAxis(0, (float)e->dx);
		device->setAxis(1, (float)e->dy);
	}
}

void InputComponent::handleMousePressedEvent(int nr, bool pressed)
{
	QTInputDevices* device = inputManager_->GetMouseAndKeyboard();

	if(device != nullptr)
	{
		device->setMouseButton(nr, pressed);
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