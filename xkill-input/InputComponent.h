#pragma once

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventType.h>
#include "dllInput.h"

#include <xkill-utilities/LeanWindows.h>

#include <vector>

class InputManager;
struct InputAttribute;

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
	virtual bool init(HWND windowHandle, std::vector<InputAttribute>* inputAttributes, std::string configFilePath, float searchTime = 10.0f);

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	InputManager*					inputManager_;
	HWND							windowHandle_;
	std::vector<InputAttribute>*	inputAttributes_;
	float							newDeviceSearchTimer_;	//! Timer until next search for new input devices
	float							searchTime_;			//! How long time there will be between searches for new input devices

	virtual void handleInput(float delta);

	virtual void handleRumbleEvent(Event_Rumble* e);
	virtual void handleMouseMoveEvent(Event_MouseMove* e);
	virtual void handleKeyEvent(char key, bool pressed);
};