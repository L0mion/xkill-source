#include "InputDevice.h"

InputDevice::InputDevice(GUID deviceGUID, std::string name, unsigned int playerID)
{
	deviceGUID_ = deviceGUID;
	name_ = name;
	playerID_ = playerID;

	createObjectVectors();
}

InputDevice::~InputDevice()
{
	std::vector<InputObject*>::iterator it = inputObjects_.begin();
	for(; it != inputObjects_.end(); it++)
		SAFE_DELETE(*it);

	inputObjects_.clear();

	axes_.clear();
	buttons_.clear();
	triggers_.clear();
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

float InputDevice::getFloatValue(int mapping, bool useSesitivity)
{
	float maxValue = 0.0f;
	int index = 0;
	float value = 0.0f;

	for(unsigned int i = 0; i < floatObjects_[mapping].size(); i++)
	{
		index = floatObjects_[mapping][i];
		value = inputObjects_[index]->getValueFloat();

		if(useSesitivity)
			value *= inputObjects_[index]->getSensitivity();

		if(std::abs(value) > maxValue)
		{
			maxValue = value;
		}
	}

	return maxValue;
}

bool InputDevice::getBoolValue(int mapping)
{
	for(unsigned int i = 0; i < boolObjects_[mapping].size(); i++)
		if(inputObjects_[boolObjects_[mapping][i]]->getValueBool())
			return true;

	return false;
}

bool InputDevice::getBoolReleased(int mapping)
{
	for(unsigned int i = 0; i < boolObjects_[mapping].size(); i++)
		if(inputObjects_[boolObjects_[mapping][i]]->getValueBoolReleased())
			return true;

	return false;
}

InputButtonObject* InputDevice::getButtonObject(unsigned int index)
{
	if(index >= buttons_.size())
		return nullptr;

	return buttons_[index];
}

void InputDevice::createObjectVectors()
{
	boolObjects_.clear();

	for(int i = 0; i < InputBoolAction::ACTION_B_LAST; i++)
	{
		boolObjects_.push_back(std::vector<int>());

		for(unsigned int j = 0; j < inputObjects_.size(); j++)
		{
			if(inputObjects_[j]->hasBoolMapping(i))
			{
				boolObjects_[i].push_back(j);
			}
		}
	}

	floatObjects_.clear();

	for(int i = 0; i < InputFloatAction::ACTION_F_LAST; i++)
	{
		floatObjects_.push_back(std::vector<int>());

		for(unsigned int j = 0; j < inputObjects_.size(); j++)
		{
			if(inputObjects_[j]->hasFloatMapping(i))
			{
				floatObjects_[i].push_back(j);
			}
		}
	}
}
