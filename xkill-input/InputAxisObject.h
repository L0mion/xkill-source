#pragma once

#include "InputObject.h"

#include <cmath>

//! Contains data and settings from a device axis
/*!
Contains the data from an axis but formatted to the interval
(-1.0f, 1.0f). Also has settings for dead zone and if the axis
should be inverted or not.
*/

class InputAxisObject : public InputObject
{
public:
	/*!
	\param minValue The smallest value that unformatted input can have
	\param maxValue The largest value that unformatted input can have
	*/
	InputAxisObject(int minValue, int maxValue);
	virtual ~InputAxisObject(void);

	void AddValue(float value);

	//! Sets value without formatting
	/*!
	Value should be within -1.0f and 1.0f and will be capped if not.
	*/
	void SetValue(float value);
	//! Sets and formats input value
	/*!
	Value should be within minValue and maxValue and will be capped otherwise.
	*/
	void SetValue(int value);
	float GetValue();

	virtual float getValueFloat();
	virtual bool getValueBool();

	/*!
	\return AXIS_OBJECT
	*/
	virtual InputObjectType GetType();

	void setDeadZone(float deadZone);
	float getDeadZone();

private:
	float deadZone_;
	int maxValue_, minValue_;

	float value_;

	//! Formats the value to the interval (-1.0f, 1.0f)
	float formatValue(int value);
};

