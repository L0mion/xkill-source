#include "InputTriggerObject.h"

InputTriggerObject::InputTriggerObject(int minValue, int maxValue)
{
	minValue_ = minValue;
	maxValue_ = maxValue;

	deadZone_ = 0.25f;
	triggerValue_ = 0.75;

	value_ = 0.0f;
}

InputTriggerObject::~InputTriggerObject(void)
{
}

void InputTriggerObject::SetValue(float value)
{
	if(value > 1.0f)
		value_ = 1.0f;
	else if(value < 0.0f)
		value_ = 0.0f;
	else
		value_ = value;
}

void InputTriggerObject::SetValue(int value)
{
	value_ = formatValue(value);
}

float InputTriggerObject::GetValue()
{
	return value_;
}

bool InputTriggerObject::IsTriggered()
{
	return (value_ > triggerValue_);
}

InputObject::InputObjectType InputTriggerObject::GetType()
{
	return TRIGGER_OBJECT;
}

void InputTriggerObject::SetDeadZone(float deadZone)
{
	if(deadZone > 1.0f)
		deadZone_ = 1.0f;
	else if(deadZone < 0.0f)
		deadZone_ = 0.0f;
	else
		deadZone_ = deadZone;
}

void InputTriggerObject::SetTriggerValue(float triggerValue)
{
	if(triggerValue > 1.0f)
		triggerValue_ = 1.0f;
	else if(triggerValue < 0.0f)
		triggerValue_ = 0.0f;
	else
		triggerValue_ = triggerValue;
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
