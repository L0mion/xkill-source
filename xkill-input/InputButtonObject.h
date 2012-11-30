#pragma once

#include "InputObject.h"

//! Contains data and settings from a device button
/*!
Contains the data from an axis but formatted to a bolean value.
*/

class InputButtonObject : public InputObject
{
public:
	InputButtonObject(void);
	virtual ~InputButtonObject(void);

	void SetValue(bool value);
	void SetValue(int value);

	/*!
	\return BUTTON_OBJECT
	*/
	virtual InputObjectType GetType();

	//! Returns if the button is down
	bool isDown();
	//! Returns if the button was since last update
	bool isReleased();

private:
	bool value_;
	bool prevValue_;
};

