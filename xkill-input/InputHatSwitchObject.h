#pragma once

#include "InputObject.h"

//! Contains data and settings from a device hat switch (d-pad)
/*!
Handles the data for the d-pad as four buttons corresponding to the four
directions (up, right, down, left).
*/

class InputHatSwitchObject : public InputObject
{
public:
	InputHatSwitchObject(void);
	virtual ~InputHatSwitchObject(void);

	void SetButton(int button, bool pressed);

	/*!
	\return HAT_SWITCH_OBJECT
	*/
	virtual InputObjectType GetType();

	bool buttonDown(int button);
	bool buttonReleased(int button);

	virtual float getValueFloat() {return 0.0f;};
	virtual bool getValueBool() {return false;};

private:
	bool buttonStates[4];
	bool prevButtonStates[4];

public:
	//! Used for indexing the buttons
	enum HatSwitchButtons
	{
		HAT_SWITCH_UP,
		HAT_SWITCH_RIGHT,
		HAT_SWITCH_DOWN,
		HAT_SWITCH_LEFT
	};
};

