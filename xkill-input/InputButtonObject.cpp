#include "InputButtonObject.h"

InputButtonObject::InputButtonObject(void)
{
	value_ = false;
	prevValue_ = false;
}

InputButtonObject::~InputButtonObject(void)
{
}

void InputButtonObject::SetValue(bool value)
{
	if(prevValue_ != value)
		int t = 0;

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
