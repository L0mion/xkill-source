#pragma once

#include "InputObject.h"

//! Contains data and settings from a device button
/*!
Contains the data from an axis but formatted to a bolean value.
*/

class InputButtonObject : public InputObject
{
public:
	InputButtonObject(char key);
	virtual ~InputButtonObject(void);

	void SetValue(bool value);
	void SetValue(int value);

	/*!
	\return BUTTON_OBJECT
	*/
	virtual InputObjectType GetType();

	virtual float getValueFloat();
	virtual bool getValueBool();
	virtual bool getValueBoolPressed();
	virtual bool getValueBoolReleased();

	//! Returns if the button is down
	bool isDown();
	//! Returns if the button was since last update
	bool isReleased();

	char getKey();
	void setKey(char key);

private:
	bool value_;
	bool prevValue_;

	char keyChar_;	//Make a subclass instead with added functionality
};

