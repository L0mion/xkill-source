#include "InputDevice.h"

InputDevice::InputDevice(GUID deviceGUID, std::string name, unsigned int playerID)
{
	deviceGUID_ = deviceGUID;
	name_ = name;
	playerID_ = playerID;
}

InputDevice::~InputDevice()
{
	std::vector<InputObject*>::iterator it = inputObjects_.begin();
	for(; it != inputObjects_.end(); it++)
		SAFE_DELETE(*it);

	inputObjects_.clear();
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

float InputDevice::getFloatValue(int mapping)
{
	float maxValue = 0.0f;
	
	for(unsigned int i = 0; i < inputObjects_.size(); i++)
		if(inputObjects_[i]->hasFloatMapping(mapping))
			if(inputObjects_[i]->getValueFloat() > maxValue)
				maxValue = inputObjects_[i]->getValueFloat();

	return maxValue;
}

bool InputDevice::getBoolValue(int mapping)
{
	for(unsigned int i = 0; i < inputObjects_.size(); i++)
		if(inputObjects_[i]->hasBoolMapping(mapping))
			if(inputObjects_[i]->getValueBool())
				return true;

	return false;
}

InputButtonObject* InputDevice::getButtonObject(int index)
{
	if(index >= buttons_.size())
		return nullptr;

	return buttons_[index];
}