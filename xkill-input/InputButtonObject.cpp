#include "InputButtonObject.h"

InputButtonObject::InputButtonObject(char key)
{
	value_ = false;
	prevValue_ = false;
	keyChar_ = key;
}

InputButtonObject::~InputButtonObject(void)
{
}

void InputButtonObject::SetValue(bool value)
{
	prevValue_ = value_;
	value_ = value;
}

void InputButtonObject::SetValue(int value)
{
	SetValue(value > 0);
}

InputObject::InputObjectType InputButtonObject::GetType()
{
	return BUTTON_OBJECT;
}

bool InputButtonObject::isDown()
{
	return value_;
}

bool InputButtonObject::isReleased()
{
	return (!value_ && prevValue_);
}

float InputButtonObject::getValueFloat()
{
	float value = (float)value_;

	if(inverted_)
		value *= (-1);

	return value;
}

bool InputButtonObject::getValueBool()
{
	return isDown();
}

bool InputButtonObject::getValueBoolReleased()
{
	return isReleased();
}

char InputButtonObject::getKey()
{
	return keyChar_;
}

void InputButtonObject::setKey(char key)
{
	keyChar_ = key;
}
