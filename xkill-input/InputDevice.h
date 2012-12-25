#pragma once

#include <vector>

#include <InitGuid.h>

#include "KeyMapper.h"

#include "InputButtonObject.h"
#include "InputAxisObject.h"
#include "InputTriggerObject.h"

#include "InputActions.h"

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

//! An interface for wrappers of Direct Input and XInput devices

class InputDevice
{
	friend class KeyMapper;

public:
	//! Lists the capabilities of the device
	/*!
	Lists how many of each object the device has and if it has force feedback.
	*/
	struct InputDeviceLayout
	{
		int nrOfAxes;
		int nrOfButtons;
		int nrOfTriggers;
		int nrOfHatSwitches;
		bool hasForceFeedback;
	};

	//! Used to differentiate between different kinds of devices
	enum InputDeviceType
	{
		XINPUT_DEVICE,
		DIRECT_INPUT_DEVICE,
		DIRECT_INPUT_MOUSE,
		DIRECT_INPUT_KEYBOARD,
		QT_INPUT_DEVICE,
	};

	InputDevice(GUID deviceGUID, std::string name, unsigned int playerID = -1);
	virtual ~InputDevice(void);

	//! Updates the device
	/*!
	\param deltaTime Time since last call
	*/
	virtual void Update(float deltaTime) = 0;

	virtual void RunForceFeedback() = 0;
	virtual void StopForceFeedback() = 0;
	//! Sets the intensity of each motor
	/*!
	Handles up to two force feedback motors. If only one motor is available then it'll take an average of both values;
	\param leftMotorScale Scales the left motor intensity. From 0.0f to 1.0f.
	\param rightMotorScale Scales the right motor intensity. From 0.0f to 1.0f.
	*/
	virtual void SetForceFeedback(float leftMotorScale, float rightMotorScale) = 0;
	virtual bool IsForceFeedbackCapable() = 0;

	virtual InputDeviceLayout GetLayout();

	virtual InputDeviceType GetType() = 0;
	virtual GUID GetGUID();
	virtual std::string GetName();

	virtual void setPlayerID(int playerID);
	virtual int getPlayerID();
	
	//! Returns the largest float value of objects that is mapped to that number
	virtual float getFloatValue(int mapping, bool useSensitivity = false);
	//! Returns if any object that is mapped to that number is activated
	virtual bool getBoolValue(int mapping);
	//! Returns if any object that is mapped to that number was released
	virtual bool getBoolReleased(int mapping);

	//! Sets standard mappings
	/*!
	Note: Should only be called if the device don't have any previous mappings
	as it doesn't reset previous mappings. The result could be that multiple 
	mappings will be present at the same time.
	*/
	virtual void setStandardMappings() = 0;

	void createObjectVectors();

protected:
	InputDeviceLayout inputLayout_;
	GUID deviceGUID_;
	std::string name_;
	unsigned int playerID_;

	std::vector<InputAxisObject*> axes_;				//Should perhaps use an inputstate to store this instead?
	std::vector<InputButtonObject*> buttons_;
	std::vector<InputTriggerObject*> triggers_;
	std::vector<InputObject*> inputObjects_;

	std::vector<std::vector<int>> floatObjects_;
	std::vector<std::vector<int>> boolObjects_;

	//! Updates the input object to the latest state
	virtual void updateState() = 0;
	//! Creates a input layout
	virtual void createInputLayout() = 0;
	//! Creates all the input objects using the input layout as guide
	virtual void createInputObjectsFromLayout() = 0;

	virtual InputButtonObject* getButtonObject(unsigned int index);
};

