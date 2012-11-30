#pragma once

//! An interface for input objects
/*!
An input object contains state data from an object on the 
device, functions that format the data as well as settings
concerning the object.
*/

class InputObject
{
public:
	//! Used to differentiate between different input objects
	enum InputObjectType
	{
		AXIS_OBJECT,
		BUTTON_OBJECT,
		HAT_SWITCH_OBJECT,
		TRIGGER_OBJECT
	};

	InputObject(void);
	virtual ~InputObject(void);

	virtual InputObjectType GetType() = 0;
};

