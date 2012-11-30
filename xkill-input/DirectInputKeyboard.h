#pragma once

#include "DirectInputDevice.h"

//! A wrapper for a Direct Input keyboard device
/*!
The big difference from DirectInputDevice is that this class uses a array of long
as input from Direct Input instead of a JoyState struct.
*/

class DirectInputKeyboard : public DirectInputDevice
{
public:
	DirectInputKeyboard(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name);
	virtual ~DirectInputKeyboard(void);

	virtual bool Init(HWND hWindow);

	virtual void Update(float deltaTime);

	virtual InputDeviceType GetType();

protected:
	virtual void updateState();
};

