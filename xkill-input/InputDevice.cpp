#include "InputDevice.h"

InputDevice::InputDevice(GUID deviceGUID, std::string name, unsigned int playerID)
{
	deviceGUID_ = deviceGUID;
	name_ = name;
	playerID_ = playerID;
}

InputDevice::~InputDevice()
{
}

InputDevice::InputState InputDevice::GetState()
{
	InputState state;

	state.axes = axes_;
	state.buttons = buttons_;
	state.hatSwitches = hatSwitches_;
	state.triggers = triggers_;

	return state;
}

InputDevice::InputDeviceLayout InputDevice::GetLayout()
{
	return inputLayout_;
}

GUID InputDevice::GetGUID()
{
	return deviceGUID_;
}

std::string InputDevice::GetName()
{
	return name_;
}

void InputDevice::setPlayerID(int playerID)
{
	playerID_ = playerID;
}

int InputDevice::getPlayerID()
{
	return playerID_;
}