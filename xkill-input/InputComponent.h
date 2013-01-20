#pragma once

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventType.h>
#include "dllInput.h"

#ifndef HWND_FORWARD_DECLARATION
#define HWND_FORWARD_DECLARATION
typedef struct HWND__* HWND;
#endif

#include <vector>

class InputManager;
struct Attribute_Input;

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
	virtual bool init(HWND windowHandle, std::string configFilePath, float searchTime = 10.0f);

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:
	InputManager*					inputManager_;
	HWND							windowHandle_;
	float							newDeviceSearchTimer_;	//! Timer until next search for new input devices
	float							searchTime_;			//! How long time there will be between searches for new input devices

	void handleInput(float delta);

	void setupPlayerControllerConnection();

	void handleRumbleEvent(Event_Rumble* e);
	void handleMouseMoveEvent(Event_MouseMove* e);
	void handleKeyEvent(char key, bool pressed);
};