#pragma once

#include "DirectInputDevice.h"

class InputObject;
class InputAxisObject;
class InputButtonObject;
class InputTriggerObject;

//! A wrapper for a Direct Input mouse device
/*!
The big difference from DirectInputDevice is that this class uses a MouseState struct
as input from Direct Input instead of a JoyState struct.

#NOTE: This class is not updated to comply with latest interface changes.
*/

class DirectInputMouse : public DirectInputDevice
{
public:
	DirectInputMouse(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name, unsigned int playerID);
	virtual ~DirectInputMouse(void);

	virtual bool Init(HWND hWindow);

	virtual void Update(float deltaTime);

	virtual InputDeviceType GetType();
	
private:
	virtual void updateState();

	virtual void createInputObjectsFromLayout();
};

