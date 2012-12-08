#include "InputButtonObject.h"

InputButtonObject::InputButtonObject(void)
{
	value_ = false;
	prevValue_ = false;
	keyChar_ = 0;
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
	return (float)value_;
}

bool InputButtonObject::getValueBool()
{
	return value_;
}

char InputButtonObject::getKey()
{
	return keyChar_;
}

void InputButtonObject::setKey(char key)
{
	keyChar_ = key;
}
