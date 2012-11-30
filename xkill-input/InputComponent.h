#pragma once

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/EventType.h>
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
	virtual bool init(HWND windowHandle);

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	InputManager*	inputManager_;
	HWND			windowHandle_;

	virtual void handleEvents();
};