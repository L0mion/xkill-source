#include "InputHatSwitchObject.h"

InputHatSwitchObject::InputHatSwitchObject(void)
{
	for(int i = 0; i < 4; i++)
	{
		buttonStates[i] = false;
		prevButtonStates[i] = false;
	}
}

InputHatSwitchObject::~InputHatSwitchObject(void)
{
}

void InputHatSwitchObject::SetButton(int button, bool pressed)
{
	if(button < 4 && button >= 0)
	{
		prevButtonStates[button] = buttonStates[button];
		buttonStates[button] = pressed;
	}
}

InputObject::InputObjectType InputHatSwitchObject::GetType()
{
	return HAT_SWITCH_OBJECT;
}

bool InputHatSwitchObject::buttonDown(int button)
{
	if(button < 4 && button >= 0)
	{
		return buttonStates[button];
	}
	else
	{
		return false;
	}
}

bool InputHatSwitchObject::buttonReleased(int button)
{
	if(button < 4 && button >= 0)
	{
		return (!buttonStates[button] && prevButtonStates[button]);
	}
	else
	{
		return false;
	}
}