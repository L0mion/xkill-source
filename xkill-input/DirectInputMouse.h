#pragma once

#include "DirectInputDevice.h"

//! A wrapper for a Direct Input mouse device
/*!
The big difference from DirectInputDevice is that this class uses a MouseState struct
as input from Direct Input instead of a JoyState struct.
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

