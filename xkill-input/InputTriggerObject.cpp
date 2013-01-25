#include "InputTriggerObject.h"

InputTriggerObject::InputTriggerObject(int minValue, int maxValue)
{
	minValue_ = minValue;
	maxValue_ = maxValue;

	deadZone_ = 0.25f;
	triggerValue_ = 0.75;

	value_ = 0.0f;
	prevValue_ = 0.0f;
}

InputTriggerObject::~InputTriggerObject(void)
{
}

void InputTriggerObject::SetValue(float value)
{
	prevValue_ = value_;

	if(value > 1.0f)
		value_ = 1.0f;
	else if(value < 0.0f)
		value_ = 0.0f;
	else
		value_ = value;
}

void InputTriggerObject::SetValue(int value)
{
	prevValue_ = value_;
	value_ = formatValue(value);
}

float InputTriggerObject::GetValue()
{
	float value = value_;

	if(inverted_)
		value *= -1.0f;

	return value;
}

bool InputTriggerObject::IsTriggered()
{
	return (value_ > triggerValue_);
}

float InputTriggerObject::getValueFloat()
{
	return GetValue();
}

bool InputTriggerObject::getValueBool()
{
	return IsTriggered();
}

bool InputTriggerObject::getValueBoolPressed()
{
	return (floatToBool(value_) && !floatToBool(prevValue_));
}

bool InputTriggerObject::getValueBoolReleased()
{
	return (!floatToBool(value_) && floatToBool(prevValue_));
}

InputObject::InputObjectType InputTriggerObject::GetType()
{
	return TRIGGER_OBJECT;
}

void InputTriggerObject::setDeadZone(float deadZone)
{
	if(deadZone > 1.0f)
		deadZone_ = 1.0f;
	else if(deadZone < 0.0f)
		deadZone_ = 0.0f;
	else
		deadZone_ = deadZone;
}

float InputTriggerObject::getDeadZone()
{
	return deadZone_;
}

void InputTriggerObject::setTriggerValue(float triggerValue)
{
	if(triggerValue > 1.0f)
		triggerValue_ = 1.0f;
	else if(triggerValue < 0.0f)
		triggerValue_ = 0.0f;
	else
		triggerValue_ = triggerValue;
}

float InputTriggerObject::getTriggerValue()
{
	return triggerValue_;
}

float InputTriggerObject::formatValue(int value)
{
	double doubleTrigger = value;
	double interval = (double)(maxValue_ - minValue_);

	doubleTrigger = doubleTrigger - ((double)minValue_);
	doubleTrigger = doubleTrigger / (interval);

	if(std::abs(doubleTrigger) < deadZone_)
		doubleTrigger = 0.0;

	return (float)doubleTrigger;
}

bool InputTriggerObject::floatToBool(float value)
{
	return (std::abs(value) >= triggerValue_);
}
