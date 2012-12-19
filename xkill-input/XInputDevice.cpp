#include "XInputDevice.h"
#include <xkill-utilities/LeanWindows.h>

#include <Xinput.h>

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

	std::vector<InputTriggerObject*>::iterator it;
	for(; it != triggers_.end(); it++)
		delete (*it);

	triggers_.clear();
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
		axes_[0]->addFloatMapping(0);
		axes_[1]->addFloatMapping(1);
		axes_[2]->addFloatMapping(2);
		axes_[3]->addFloatMapping(3);
	}

	if(buttons_.size() >= 14)
	{
		buttons_[10]->addBoolMapping(3);
		buttons_[11]->addBoolMapping(6);
		buttons_[12]->addBoolMapping(5);
		buttons_[13]->addBoolMapping(4);
	}

	if(triggers_.size() >= 2)
	{
		triggers_[0]->addBoolMapping(0);
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
		axes_.push_back(new InputAxisObject(-0x7FFF, 0x7FFF));
		inputObjects_.push_back(axes_[axes_.size() - 1]);
	}

	if(axes_.size() >= 4)
		axes_[3]->setInverted(true);
	
	for(int i = 0; i < inputLayout_.nrOfButtons + inputLayout_.nrOfHatSwitches*4; i++)
	{
		buttons_.push_back(new InputButtonObject(i));
		inputObjects_.push_back(buttons_[buttons_.size() - 1]);
	}

	for(int i = 0; i < inputLayout_.nrOfTriggers; i++)
	{
		triggers_.push_back(new InputTriggerObject(0, 0xFF));
		inputObjects_.push_back(triggers_[triggers_.size() - 1]);
	}
}