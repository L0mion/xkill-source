#include "InputObject.h"

InputObject::InputObject(void)
{
	inverted_ = false;
	sensitivity_ = 2.0f;
	name_ = "<NO_NAME>";

	useAcceleration_ = false;
	inAccelerationZone_ = false;
	currentAcceleration_ = 1.0f;
	accelerationRate_ = 2.5f;
	maxAcceleration_ = 2.5f;
}

InputObject::~InputObject(void)
{
}

void InputObject::update(float delta)
{
	if(useAcceleration_)
	{
		if(inAccelerationZone_)
		{
			currentAcceleration_ += accelerationRate_ * delta;
			if(currentAcceleration_ > maxAcceleration_)
			{
				currentAcceleration_ = maxAcceleration_;
			}
		}
		else
		{
			currentAcceleration_ = 1.0f;
		}
	}
}

void InputObject::setInverted(bool inverted)
{
	inverted_ = inverted;
}

bool InputObject::isInverted()
{
	return inverted_;
}

void InputObject::setSensitivity(float sensitivity)
{
	sensitivity_ = sensitivity;
}

float InputObject::getSensitivity()
{
	return sensitivity_;
}

void InputObject::setUseAcceleration(bool useAcceleration)
{
	useAcceleration_ = useAcceleration;
}

void InputObject::setAccelerationRate(float accelerationRate)
{
	accelerationRate_ = accelerationRate;
}

void InputObject::setMaxAcceleration(float maxAcceleration)
{
	maxAcceleration_ = maxAcceleration;
}

void InputObject::setIsInAccelerationZone(bool inAccelerationZone)
{
	inAccelerationZone_ = inAccelerationZone;
}

bool InputObject::useAcceleration()
{
	return useAcceleration_;
}

float InputObject::getAcceleration()
{
	return currentAcceleration_;
}

bool InputObject::needsDelta()
{
	return false;
}

void InputObject::addMapping(int mapping)
{
	mappings_.push_back(mapping);
}

void InputObject::removeMapping(int mapping)
{
	removeMapping(&mappings_, mapping);
}

bool InputObject::hasMapping(int mapping)
{
	return hasMapping(&mappings_, mapping);
}

std::vector<int>* InputObject::getMappings()
{
	return &mappings_;
}

void InputObject::removeMapping(std::vector<int>* vector, int mapping)
{
	for(unsigned int i = 0; i < vector->size(); i++)
	{
		if(vector->at(i) == mapping)
		{
			swap(vector, i, vector->size() - 1);
			vector->pop_back();
			i--;
		}
	}
}

bool InputObject::hasMapping(std::vector<int>* vector, int mapping)
{
	for(unsigned int i = 0; i < vector->size(); i++)
		if(vector->at(i) == mapping)
			return true;

	return false;
}

void InputObject::swap(std::vector<int>* vector, int firstIndex, int secondIndex)
{
	int temp = vector->at(firstIndex);
	vector->at(firstIndex) = vector->at(secondIndex);
	vector->at(secondIndex) = temp;
}

void InputObject::setName(std::string name)
{
	name_ = name;
}

std::string InputObject::getName()
{
	return name_;
}
