#include "InputDevice.h"

#include "InputObjectArray.h"

#include "InputButtonObject.h"
#include "InputAxisObject.h"
#include "InputTriggerObject.h"

#include "KeyMapper.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EntityStorage.h>

#include "Converter.h"

ATTRIBUTES_DECLARE_ALL

InputDevice::InputDevice(GUID deviceGUID, std::string name, unsigned int playerID)
{
	ATTRIBUTES_INIT_ALL

	deviceGUID_ = deviceGUID;
	name_ = name;
	playerID_ = playerID;

	rumbleTimer_ = 0.0f;
	rumbleActive_ = false;
	sensitivityModifier_ = 1.0f;

	inputObjectArray_ = new InputObjectArray();

	//Needs to create a inputDeviceSettings attribute
	//itrInputDevice.createAttribute(settings_entity)->inputObjectArray = inputObjectArray_;

	createObjectVectors();
}

InputDevice::~InputDevice()
{
	std::vector<InputObject*>::iterator it = inputObjectArray_->inputObjects.begin();
	for(; it != inputObjectArray_->inputObjects.end(); it++)
		SAFE_DELETE(*it);

	inputObjectArray_->inputObjects.clear();
	delete inputObjectArray_;

	axes_.clear();
	buttons_.clear();
	triggers_.clear();
}

void InputDevice::Update(float deltaTime)
{
	if(IsForceFeedbackCapable() && rumbleActive_)
	{
		rumbleTimer_ -= deltaTime;

		if(rumbleTimer_ <= 0.0f)
		{
			rumbleActive_ = StopForceFeedback();
		}
	}
}

void InputDevice::RunForceFeedback(float timer)
{
	rumbleActive_ = true;
	rumbleTimer_ = timer;

	RunForceFeedback();
}

InputDevice::InputDeviceLayout InputDevice::GetLayout()
{
	return inputLayout_;
}

InputObjectArray* InputDevice::getInputObjectArray()
{
	return inputObjectArray_;
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

std::vector<std::string> InputDevice::getNamesOfMappedObjects(int mapping)
{
	std::vector<std::string> names;
	int index;

	for(unsigned int i = 0; i < mappedObjects_[mapping].size(); i++)
	{
		index = mappedObjects_[mapping][i];
		names.push_back(inputObjectArray_->inputObjects[index]->getName());
	}

	return names;
}

float InputDevice::getFloatValue(int mapping, float delta, bool useSensitivity)
{
	float maxValue = 0.0f;
	int index = 0;
	float value = 0.0f;

	for(unsigned int i = 0; i < mappedObjects_[mapping].size(); i++)
	{
		index = mappedObjects_[mapping][i];
		value = inputObjectArray_->inputObjects[index]->getValueFloat();

		if(useSensitivity)
		{
			float sensitivity;
			sensitivity = inputObjectArray_->inputObjects[index]->getSensitivity()*sensitivityModifier_;
			value *= sensitivity;

			if(inputObjectArray_->inputObjects[index]->needsDelta())
				value *= delta;
		}

		if(std::abs(value) > std::abs(maxValue))
		{
			maxValue = value;
		}
	}

	return maxValue;
}

bool InputDevice::getBoolValue(int mapping)
{
	for(unsigned int i = 0; i < mappedObjects_[mapping].size(); i++)
		if(inputObjectArray_->inputObjects[mappedObjects_[mapping][i]]->getValueBool())
			return true;

	return false;
}

bool InputDevice::getBoolReleased(int mapping)
{
	for(unsigned int i = 0; i < mappedObjects_[mapping].size(); i++)
		if(inputObjectArray_->inputObjects[mappedObjects_[mapping][i]]->getValueBoolReleased())
			return true;

	return false;
}

bool InputDevice::getBoolPressed(int mapping)
{
	for(unsigned int i = 0; i < mappedObjects_[mapping].size(); i++)
		if(inputObjectArray_->inputObjects[mappedObjects_[mapping][i]]->getValueBoolPressed())
			return true;

	return false;
}

std::vector<int> InputDevice::getMappedArray(int mapping)	//Switch to unsigned
{
	return mappedObjects_[mapping];
}

/*  //////////////////////////////////////////////////////////////////
	Hash algorithm djb2 from: http://www.cse.yorku.ca/~oz/hash.html	//
*/  //////////////////////////////////////////////////////////////////
unsigned long InputDevice::getHash()
{
	std::string str = getStandardMappingsString();

	str += Converter::IntToStr(InputAction::ACTION_LAST);

	return Converter::HashString(str);
}

InputButtonObject* InputDevice::getButtonObject(unsigned int index)
{
	if(index >= buttons_.size())
		return nullptr;

	return buttons_[index];
}

void InputDevice::createObjectVectors()
{
	mappedObjects_.clear();

	for(int i = 0; i < InputAction::ACTION_LAST; i++)
	{
		mappedObjects_.push_back(std::vector<int>());

		for(unsigned int j = 0; j < inputObjectArray_->inputObjects.size(); j++)
		{
			if(inputObjectArray_->inputObjects[j]->hasMapping(i))
			{
				mappedObjects_[i].push_back(j);
			}
		}
	}
}

void InputDevice::setSensitivityModifier(float value)
{
	sensitivityModifier_ = 0.1f+(1 - value)*0.9;
}