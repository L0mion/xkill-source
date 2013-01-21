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
	\param set if axis is a relative axis
	*/
	InputAxisObject(int minValue, int maxValue, bool relativeAxis = false);
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
	virtual bool getValueBoolReleased();

	/*!
	\return AXIS_OBJECT
	*/
	virtual InputObjectType GetType();

	void setDeadZone(float deadZone);
	float getDeadZone();

	virtual bool needsDelta();

private:
	float deadZone_;
	int maxValue_, minValue_;
	bool relativeAxis_;

	float value_;
	float prevValue_;

	//! Formats the value to the interval (-1.0f, 1.0f)
	float formatValue(int value);
	bool floatToBool(float value);
};

