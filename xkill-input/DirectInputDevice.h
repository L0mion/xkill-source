#pragma once

#include "InputDevice.h"

#include <vector>

#include <InitGuid.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL 0x80000000
#endif

#define SAFE_RELEASE(x) {if(x != NULL) x->Release(); x = NULL;} //Should probably not be here

class InputObject;
class InputAxisObject;
class InputButtonObject;
class InputTriggerObject;

//! A wrapper of a Direct Input device
/*!


Some code should probably be refactored to a DirectInputGamepad class.
*/

class DirectInputDevice : public InputDevice
{
protected:
	//! Used for passing data from callback function
	struct EnumObjectsStruct
	{
		EnumObjectsStruct()
		{
			nrOfFFObjects = 0;
		}

		int nrOfFFObjects;
		std::vector<std::string> buttonNames;
		std::vector<std::string> axisNames;
		std::vector<std::string> hatSwitchNames;
	};

private:
	DirectInputDevice();
public:
	DirectInputDevice(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name, unsigned int playerID, bool isXInputController = false);
	virtual ~DirectInputDevice(void);

	//! Initiates the Direct Input device
	virtual bool Init(HWND hWindow);

	virtual void Update(float deltaTime);

	virtual void RunForceFeedback();
	virtual bool StopForceFeedback();
	virtual void SetForceFeedback(float leftMotorScale, float rightMotorScale);
	virtual bool IsForceFeedbackCapable();

	virtual void setStandardMappings();

	virtual InputDeviceType GetType();

	//! Callback function used when enumerating the device objects
	/*!
	Will be passed to DirectInputDevices' EnumObjects function. Currently only used to enumerate
	how many force feedback motors is present in the device. 

	Could potentially be used to create a custom dataformat for input.
	*/
	static BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* object, LPVOID pvRef);

	//static DIOBJECTDATAFORMAT CreateDataFormat(const DIDEVICEOBJECTINSTANCE* object, int offset);
	//static BOOL SetupAxisObject(const DIDEVICEOBJECTINSTANCE* object, LPDIRECTINPUTDEVICE8 device);

protected:
	LPDIRECTINPUTDEVICE8 device_;
	LPDIRECTINPUTEFFECT effect_;
	//! Used to map axes to the DirectInput JoyStates' axes
	std::vector<int> axesIndexArray_;

	bool hasFF_;
	bool isFFTurnedOn_;
	int nrOfFFMotors_;
	float leftFFMotorScale_;
	float rightFFMotorScale_;
	bool isXInputController_;

	virtual std::string getStandardMappingsString();

	//! Creates a force feedback effect
	/*!
	Currently uses a constant power effect.
	*/
	virtual HRESULT createEffect(int nrFFObjects);
	//virtual HRESULT setDeviceDataFormat(std::vector<DIOBJECTDATAFORMAT> ODFArray);

	virtual void updateState();
	virtual void createInputLayout();
	virtual void createInputObjectsFromLayout();
	virtual void createInputObjectsFromLayout(EnumObjectsStruct* eos);
	//! Creates the axes objects 
	/*!
	Fills the axesIndexArray that is used to map internal representation of axes
	to that one of the predefined DirectInput JoyState. Also makes sure that all 
	axes listed is actually used on the controller.

	Could potentially fail to list axes on the controller if they are at a certain
	position at start up. Unable to replicate this however so maybe not a problem.
	*/
	virtual void createAxes(EnumObjectsStruct* eos);
};

