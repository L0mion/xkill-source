#include "InputComponent.h"

#include <xkill-utilities/Util.h>
#include "InputManager.h"
#include "QTInputDevices.h"

ATTRIBUTES_DECLARE_ALL;

InputComponent::InputComponent()
{
	ATTRIBUTES_INIT_ALL;

	SUBSCRIBE_TO_EVENT(this, EVENT_INPUT_DEVICE_SEARCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_RUMBLE);
	SUBSCRIBE_TO_EVENT(this, EVENT_MOUSE_MOVE);
	SUBSCRIBE_TO_EVENT(this, EVENT_KEY_PRESS);
	SUBSCRIBE_TO_EVENT(this, EVENT_MOUSE_PRESS);
	SUBSCRIBE_TO_EVENT(this, EVENT_MOUSE_WHEEL);

	inputManager_ = nullptr;
	newDeviceSearchTimer_ = 0.0f;
}

InputComponent::~InputComponent()
{
	UNSUBSCRIBE_TO_EVENTS(this);
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
		bool shiftPressed = ekp->shiftPressed;
		bool tabPressed = ekp->tabPressed;

		handleKeyEvent(keyEnum, isPressed, shiftPressed, tabPressed);
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
	if(type == EVENT_MOUSE_WHEEL)
	{
		Event_MouseWheel* emw = static_cast<Event_MouseWheel*>(e);

		QTInputDevices* device = inputManager_->GetMouseAndKeyboard();
		
		if(device != nullptr)
		{
			if(emw->value >= 0)
			{
				//device->setScrollButton(true);
				handleKeyEvent('â', true);
			}
			else
			{
				//device->setScrollButton(false);
				handleKeyEvent('ô', true);
			}
		}
	}
}

void InputComponent::onUpdate(float delta)
{
	inputManager_->Update(delta);

	handleInput(delta);
}

void InputComponent::handleInput(float delta)
{
	delta = SETTINGS->trueDeltaTime;

	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

		if(ptr_player->ptr_inputDevice.isEmpty())
			continue;

		InputDevice* device = ptr_player->ptr_inputDevice->device;
		AttributePtr<Attribute_Input> input = ptr_player->ptr_input;

		if(device == nullptr)
			continue;

		float sensitivityModifier = device->getFloatValue(InputAction::ACTION_F_LOW_SENSITIVITY, delta);
		device->setSensitivityModifier(sensitivityModifier);

		if(sensitivityModifier < 0.001f)
		{
			if(device->getBoolValue(InputAction::ACTION_B_LOW_SENSITIVITY))
			{
				device->setSensitivityModifier(0.7f);
				ptr_player->zoomTimer.update(delta);
				if(ptr_player->zoomTimer.hasTimerExpired())
				{
					ptr_player->zoomTimer.zeroTimer();
				}
			}
			else
			{
				ptr_player->zoomTimer.update(-delta);
				if(ptr_player->zoomTimer.getTimeLeft() >= ptr_player->zoomTimer.getStartTime())
				{
					ptr_player->zoomTimer.resetTimer();
				}
			}

			float scale = 1.0f;
			if(ptr_player->zoomTimer.getStartTime() > 0.0f)
				scale = 1.0f / ptr_player->zoomTimer.getStartTime();

			sensitivityModifier = (1.0f - ptr_player->zoomTimer.getTimeLeft()*(scale));
		}

		ptr_player->ptr_camera->fieldOfViewModifier = 1.0f - sensitivityModifier/3;

		input->position.x = device->getFloatValue(InputAction::ACTION_F_WALK_LR, delta);
		input->position.y = device->getFloatValue(InputAction::ACTION_F_WALK_FB, delta);
		//input->rotation.x = device->getFloatValue(InputAction::ACTION_F_LOOK_LR, delta, true);
		//input->rotation.y = device->getFloatValue(InputAction::ACTION_F_LOOK_UD, delta, true);

		if(device->GetType() != device->QT_INPUT_DEVICE)
		{
			Float2 rot = device->getFormattedFloatPair(InputAction::ACTION_F_LOOK_LR, InputAction::ACTION_F_LOOK_UD, delta, true);
			input->rotation.x = rot.x;
			input->rotation.y = rot.y;
		}
		else
		{
			input->rotation.x = device->getFloatValue(InputAction::ACTION_F_LOOK_LR, delta, true);
			input->rotation.y = device->getFloatValue(InputAction::ACTION_F_LOOK_UD, delta, true);
		}

		input->fire =		 device->getBoolValue(InputAction::ACTION_B_FIRE);
		input->firePressed = device->getBoolPressed(InputAction::ACTION_B_FIRE);
							 
		input->jetpack =	 device->getBoolValue(InputAction::ACTION_B_JUMP_JETPACK);
		input->jump =		 device->getBoolPressed(InputAction::ACTION_B_JUMP_JETPACK);

		input->changeAmmunitionType = 0;
		if(device->getBoolReleased(InputAction::ACTION_B_NEXT_AMMUNITIONTYPE))
			input->changeAmmunitionType++;
		if(device->getBoolReleased(InputAction::ACTION_B_PREV_AMMUNITIONTYPE))
			input->changeAmmunitionType--;

		input->changeFiringMode = 0;
		if(device->getBoolReleased(InputAction::ACTION_B_NEXT_FIRINGMODE))
			input->changeFiringMode++;
		if(device->getBoolReleased(InputAction::ACTION_B_PREV_FIRINGMODE))
			input->changeFiringMode--;

		if(ptr_player->jetHackPair.first) // if jethack active
		{
			if(device->getBoolPressed(InputAction::ACTION_B_JUMP_JETPACK))
			{
				SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_JETPACK, itrPlayer.ownerIdAt(ptr_player.index())));//, ptr_player->ptr_render->ptr_spatial->ptr_position->position, true));
			}
			else if(device->getBoolReleased(InputAction::ACTION_B_JUMP_JETPACK))
			{
				SEND_EVENT(&Event_StopSound(XKILL_Enums::Sound::SOUND_JETPACK, itrPlayer.ownerIdAt(ptr_player.index())));
			}
		}

		input->sprint =	device->getBoolValue(InputAction::ACTION_B_SPRINT);

		input->reload =	device->getBoolPressed(InputAction::ACTION_B_RELOAD);

		if(!ptr_player->detectedAsDead)
		{
			ptr_player->showScoreboard = device->getBoolValue(InputAction::ACTION_B_SHOW_SCOREBOARD);
		}

		if(device->getBoolValue(InputAction::ACTION_B_TIME_SPEED_UP))
		{
			SETTINGS->setTimeScale(SETTINGS->timeScale() + delta);
		}
		if(device->getBoolValue(InputAction::ACTION_B_TIME_SPEED_DOWN))
		{
			SETTINGS->setTimeScale(SETTINGS->timeScale() - delta);
		}


		//if(device->getBoolReleased(InputAction::ACTION_B_TOGGLE_MUTE_SOUND))
		//	SEND_EVENT(&Event_PlaySound(-1, true));

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

		normalizeVector(input->position.x, input->position.y);	// Normalize walk vector, the character shouldn't move faster than set speed

		if(device->GetType() == device->QT_INPUT_DEVICE) // Need to update QT devices since they're event based
		{
			QTInputDevices* qtDevice = static_cast<QTInputDevices*>(device);

			qtDevice->setAxesToZero();
			qtDevice->updateButtons();
			qtDevice->updateScroll();
		}
	}
}

void InputComponent::normalizeVector(float& x, float& y)
{
	float length = std::sqrt(x*x + y*y);

	if(length > 1.0f)
	{
		x = x/length;
		y = y/length;
	}
}

void InputComponent::setupPlayerControllerConnection()
{
	int playerSize = itrPlayer.count();
	int inputDeviceSize = itrInputDevice.count();

	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

		if(itrInputDevice.hasNext())
		{
			ptr_player->ptr_inputDevice = itrInputDevice.getNext();
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

void InputComponent::handleKeyEvent(char key, bool pressed, bool shiftPressed, bool tabPressed)
{
	QTInputDevices* device = inputManager_->GetMouseAndKeyboard();
		
	if(device != nullptr)
	{
		if(shiftPressed)
			device->setButton('é', pressed);
		else if(tabPressed)
			device->setButton('ê', pressed);
		else
			device->setButton(key, pressed);
	}
}

void InputComponent::resetMovementInput()
{
	while(itrPlayer.hasNext())
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

		if(ptr_player->ptr_inputDevice.isEmpty())
			continue;

		AttributePtr<Attribute_Input> input = ptr_player->ptr_input;
		input->position = Float2(0.0f, 0.0f);
		input->rotation = Float2(0.0f, 0.0f);
	}
}