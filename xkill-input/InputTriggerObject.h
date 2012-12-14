#pragma once

#include "InputObject.h"

#include <cmath>

//! Contains data and settings from a device trigger
/*!
Contains the data from a trigger but formatted to the interval
(0.0f, 1.0f). Also has settings for dead zone and a value where
the trigger is triggered (if counted as a button).
*/

class InputTriggerObject : public InputObject
{
public:
	InputTriggerObject(int minValue, int maxValue);
	virtual ~InputTriggerObject(void);

	void SetValue(float value);
	void SetValue(int value);
	float GetValue();
	bool IsTriggered();

	float getValueFloat();
	bool getValueBool();

	virtual InputObjectType GetType();

	void setDeadZone(float deadZone);
	float getDeadZone();
	void setTriggerValue(float triggerValue);
	float getTriggerValue();

private:
	float triggerValue_;
	float deadZone_;
	int maxValue_, minValue_;

	float value_;

	float formatValue(int value);
};

