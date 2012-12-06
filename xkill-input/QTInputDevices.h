#pragma once

#include "InputDevice.h"

class QTInputDevices : public InputDevice
{
public:
	QTInputDevices(GUID deviceGUID, std::string name, unsigned int playerID = -1);
	~QTInputDevices();

	virtual void Update(float deltaTime);

	virtual void RunForceFeedback();
	virtual void StopForceFeedback();
	virtual void SetForceFeedback(float leftMotorScale, float rightMotorScale);
	virtual bool IsForceFeedbackCapable();

	virtual InputDeviceType GetType();

	void setAxis(unsigned int index, float value);
	void setButton(char key, bool value);

	void setAxesToZero();

protected:

	virtual void updateState();
	virtual void createInputLayout();
	virtual void createInputObjectsFromLayout();
};