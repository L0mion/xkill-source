#include "InputObject.h"

InputObject::InputObject(void)
{
	inverted_ = false;
	sensitivity_ = 3.0f;
}

InputObject::~InputObject(void)
{
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

void InputObject::addFloatMapping(int mapping)
{
	floatMappings_.push_back(mapping);
}

void InputObject::addBoolMapping(int mapping)
{
	boolMappings_.push_back(mapping);
}

void InputObject::removeFloatMapping(int mapping)
{
	removeMapping(&floatMappings_, mapping);
}

void InputObject::removeBoolMapping(int mapping)
{
	removeMapping(&boolMappings_, mapping);
}

bool InputObject::hasFloatMapping(int mapping)
{
	return hasMapping(&floatMappings_, mapping);
}

bool InputObject::hasBoolMapping(int mapping)
{
	return hasMapping(&boolMappings_, mapping);
}

std::vector<int>* InputObject::getFloatMappings()
{
	return &floatMappings_;
}

std::vector<int>* InputObject::getBoolMappings()
{
	return &boolMappings_;
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
