#pragma once

#include "InputDevice.h"

#include <windows.h>
#include <Xinput.h>


//! A wrapper for a XInput device
/*!

*/

class XInputDevice : public InputDevice
{
public:
	XInputDevice(int deviceNr, GUID deviceGUID, std::string name);
	virtual ~XInputDevice(void);

	virtual void Update(float deltaTime);

	virtual void RunForceFeedback();
	virtual void StopForceFeedback();
	void SetForceFeedback(float leftMotorScale, float rightMotorScale);
	virtual bool IsForceFeedbackCapable();

	/*!
	\return XINPUT_DEVICE 
	*/
	virtual InputDeviceType GetType();

private:
	int deviceNr_;
	int rightFFMotor_, leftFFMotor_;
	bool forceFeedbackOn_;

	virtual void updateState();
	virtual void createInputLayout();
	virtual void createInputObjectsFromLayout();
};

