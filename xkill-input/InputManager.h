#pragma once

#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/EventType.h>

#include "InputDevice.h"
#include "DirectInputDevice.h"
#include "DirectInputMouse.h"
#include "DirectInputKeyboard.h"
#include "XInputDevice.h"
#include "QTInputDevices.h"

#include "Converter.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

///////Needed for Direct Input //////
#include <InitGuid.h>			   //
								   //
#define DIRECTINPUT_VERSION 0x0800 //
#include <dinput.h>				   //
/////////////////////////////////////

//Used for IsXInputDevice function//
#include <wbemidl.h>			  //
#include <oleauto.h>			  //
#include <stdio.h>				  //
////////////////////////////////////

#define SAFE_RELEASE(x) {if(x != NULL) x->Release(); x = NULL;} //Should probably not be here, needed for isXInputDevice

class FileParser;

//! Detects and manages devices and their input.
/*!
Enumerates all gamepads connected to the computer and sets them up for input. It also adds the system mouse and system
keyboard as devices. When updated it takes the current state of each device and handles the input. Currently this means
listening to button presses and joystick positions and give force feedback accordingly. 

Both Direct Input and up to four XInput devices can be handled. If more than four XInputs is conected then some of them
will be handled through Direct Input with a few constraints.
*/

class InputManager
{
public:

	//!Used to transfer data from the EnumDevices Callback function.
	/*!
	Stores the guid and name of all the devices that it finds. This is later used to create and name devices.
	*/

	struct EnumDevicesStruct
	{
		std::vector<GUID> deviceInstanceGUIDs;
		std::vector<GUID> deviceProductGUIDs;
		std::vector<std::string> deviceNames;
	};

	InputManager();
	virtual ~InputManager(void);

	//! Initiates devices
	/*!
	Creates the Direct Input object and creates and sets up all gamepads connected to the computer. The system mouse and keyboard
	is also added as Direct Input devices.

	Needs a handle to the instance, but will get this from the window using "GetWindowLong(hWnd, GWL_HINSTANCE)".
	
	\param hWindow The handle to current window
	\param configFilePath The path of the config file directory
	*/
	bool InitInput(HWND hWindow, std::string configFilePath);

	//! Adds newly plugged in gamepads
	/*!
	Enumerates all gamepads that is plugged into the computer. If a controller hasn't been added before then it will create and 
	adds it.

	\param hWindow The handle to current window
	*/
	int UpdateNumberOfGamepads(HWND hWindow);

	//! Updates all devices and handles their input
	/*!
	\param deltaTime Time since last call
	*/
	void Update(float deltaTime);

	InputDevice* GetDevice(unsigned int playerID);
	QTInputDevices* GetMouseAndKeyboard();
	
	//! Generates a string with the states of all devices
	/*!
	A string with information of the states of all devices.
	Used when debugging.
	*/
	std::string GetInputInformationString();

	//! Callback function for the Direct Input EnumDevices function
	/*!
	Fills a EnumDevicesStruct with data about all the devices that is found.

	\param device Device Instance of a found device
	\param pvRef Void pointer to an object
	*/
	static BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* device, LPVOID pvRef);

private:
	//! Direct Input object
	/*!
	Used for enumeration of devices and creation of Direct Input devices.
	*/
	LPDIRECTINPUT8 dInput_;
	//! Array of wrapped input devices
	std::vector<InputDevice*> devices_;
	QTInputDevices* mouseAndKeyboard_;
	//! Number of XInput devices added
	/*!
	Shouldn't be able to go above four. Four is the limit that XInput sets on how many gamepads that can be handled.
	*/
	int nrOfXInputDevices_;

	//! Creates and adds a new device
	/*!
	Checks if the device is an XInput device. If so and less than four XInput devices is connected already then it will be added as an XInput device. 
	Otherwise it will created and added as a Direct Input device.

	\param hWindow The handle to current window
	\param guid The GUID to the device to be added
	\param name The name of the device to be added
	*/
	bool addNewDevice(HWND hWindow, GUID instanceGUID, GUID productGUID, std::string name);

	int checkForNewXInputDevices();

	//! Handle input
	/*!
	Currently this is only listening to button presses and joystick positions and give force feedback accordingly. This
	should however probably either send events or modify an attribute in response to the input.
	*/
	void handleInput();

	//! Checks if a guid belongs to a XInput device.
	/*!
	Copied from msdn and used to see if a guid belongs to a XInput device.

	URL: http://msdn.microsoft.com/en-us/library/windows/desktop/ee417014%28v=vs.85%29.aspx 

	\param pGuidProductFromDirectInput The GUID to the device to be checked if it is XInput capable
	*/
	BOOL isXInputDevice( const GUID* pGuidProductFromDirectInput );
};