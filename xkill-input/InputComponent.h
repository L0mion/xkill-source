#pragma once

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-architecture/AttributeManager.h>
#include "dllInput.h"

#include <Windows.h>

class InputManager;

//! Handles input devices and their input

class DLL_I InputComponent : public IObserver
{
public:
	InputComponent();
	~InputComponent();

	//! Initialize the variables of the class
	/*!
	\param windowHandle Handle to current window
	\param searchTime Time between searches for new input devices. Default: 10s
	*/
	virtual bool init(HWND windowHandle, std::vector<InputAttribute>* inputAttributes, float searchTime = 10.0f);

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	InputManager*					inputManager_;
	HWND							windowHandle_;
	std::vector<InputAttribute>*	inputAttributes_;
	float							newDeviceSearchTimer_;	//! Timer until next search for new input devices
	float							searchTime_;			//! How long time there will be between searches for new input devices

	float pos[3];

	virtual void handleInput(float delta);
};