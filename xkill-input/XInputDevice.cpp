#include "XInputDevice.h"
#include <xkill-utilities/LeanWindows.h>

#include <Xinput.h>
#include "InputActions.h"

XInputDevice::XInputDevice(int deviceNr, GUID deviceGUID, std::string name, unsigned int playerID) : 
	InputDevice(deviceGUID, name, playerID)
{
	deviceNr_ = deviceNr;
	rightFFMotor_ = 0xFFFF;
	leftFFMotor_ = 0xFFFF;
	forceFeedbackOn_ = false;

	createInputLayout();
	createInputObjectsFromLayout();
}

XInputDevice::~XInputDevice(void)
{
	StopForceFeedback();
}

void XInputDevice::Update(float deltaTime)
{
	updateState();
}

void XInputDevice::RunForceFeedback()
{
	XINPUT_VIBRATION vibration;

	vibration.wLeftMotorSpeed = leftFFMotor_;
	vibration.wRightMotorSpeed = rightFFMotor_;

	XInputSetState(deviceNr_, &vibration);

	forceFeedbackOn_ = true;
}

void XInputDevice::StopForceFeedback()
{
	XINPUT_VIBRATION vibration;

	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;

	XInputSetState(deviceNr_, &vibration);

	forceFeedbackOn_ = false;
}

void  XInputDevice::SetForceFeedback(float leftMotorScale, float rightMotorScale)
{
	leftFFMotor_ = (int)(leftMotorScale * 0xFFFF);
	rightFFMotor_ = (int)(rightMotorScale * 0xFFFF);

	if(forceFeedbackOn_)
	{
		RunForceFeedback();
	}
}

bool XInputDevice::IsForceFeedbackCapable()
{
	return true;
}

void XInputDevice::setStandardMappings()
{
	if(axes_.size() >= 4)
	{
		axes_[0]->addFloatMapping(ACTION_F_WALK_LR);
		axes_[1]->addFloatMapping(ACTION_F_WALK_FB);
		axes_[2]->addFloatMapping(ACTION_F_LOOK_LR);
		axes_[3]->addFloatMapping(ACTION_F_LOOK_UD);
	}

	if(buttons_.size() >= 14)
	{
		buttons_[0]->addBoolMapping(ACTION_B_CHANGE_AMMUNITIONTYPE);
		buttons_[1]->addBoolMapping(ACTION_B_CHANGE_FIRINGMODE);
		buttons_[10]->addBoolMapping(ACTION_B_WALK_FORWARD);
		buttons_[11]->addBoolMapping(ACTION_B_WALK_RIGHT);
		buttons_[12]->addBoolMapping(ACTION_B_WALK_BACKWARD);
		buttons_[13]->addBoolMapping(ACTION_B_WALK_RIGHT);
	}

	if(triggers_.size() >= 2)
	{
		triggers_[0]->addBoolMapping(ACTION_B_FIRE);
	}
}

InputDevice::InputDeviceType XInputDevice::GetType()
{
	return InputDevice::InputDeviceType::XINPUT_DEVICE;
}

void XInputDevice::updateState()
{
	DWORD dwResult;    
	XINPUT_STATE state;
	
	memset(&state, 0, sizeof(XINPUT_STATE));
	dwResult = XInputGetState(deviceNr_, &state);

	if(axes_.size() >= 4)
	{
		axes_[0]->SetValue(state.Gamepad.sThumbLX);
		axes_[1]->SetValue(state.Gamepad.sThumbLY);
		axes_[2]->SetValue(state.Gamepad.sThumbRX);
		axes_[3]->SetValue(state.Gamepad.sThumbRY);
	}

	if(triggers_.size() >= 2)
	{
		triggers_[0]->SetValue(state.Gamepad.bRightTrigger);
		triggers_[1]->SetValue(state.Gamepad.bLeftTrigger);
	}

	if(buttons_.size() >= 10)
	{
		buttons_[0]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
		buttons_[1]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
		buttons_[2]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
		buttons_[3]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
		buttons_[6]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
		buttons_[7]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
		buttons_[4]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
		buttons_[5]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
		buttons_[8]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
		buttons_[9]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
	}

	//Hatswitch buttons
	if(buttons_.size() >= 14)
	{
		buttons_[10]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);	
		buttons_[11]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
		buttons_[12]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
		buttons_[13]->SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
	}
}

void XInputDevice::createInputLayout()
{
	inputLayout_.nrOfHatSwitches = 1;
	inputLayout_.nrOfButtons = 10;
	inputLayout_.nrOfAxes = 4;
	inputLayout_.nrOfTriggers = 2;
}

void XInputDevice::createInputObjectsFromLayout()
{
	for(int i = 0; i < inputLayout_.nrOfAxes; i++)
	{
		InputAxisObject* axis = new InputAxisObject(-0x7FFF, 0x7FFF);
		axes_.push_back(axis);
		inputObjects_.push_back(axis);
	}

	if(axes_.size() >= 4)
		axes_[3]->setInverted(true);
	
	for(int i = 0; i < inputLayout_.nrOfButtons + inputLayout_.nrOfHatSwitches*4; i++)
	{
		InputButtonObject* button = new InputButtonObject(i);
		buttons_.push_back(button);
		inputObjects_.push_back(button);
	}

	for(int i = 0; i < inputLayout_.nrOfTriggers; i++)
	{
		InputTriggerObject* trigger = new InputTriggerObject(0, 0xFF);
		triggers_.push_back(trigger);
		inputObjects_.push_back(trigger);
	}
}