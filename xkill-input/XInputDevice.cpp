#include "XInputDevice.h"

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

InputDevice::InputDeviceType XInputDevice::GetType()
{
	return InputDevice::InputDeviceType::XINPUT_DEVICE;
}

void XInputDevice::updateState()
{
	DWORD dwResult;    
	XINPUT_STATE state;
	
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	dwResult = XInputGetState(deviceNr_, &state);

	axes_[0].SetValue(state.Gamepad.sThumbLX);
	axes_[1].SetValue(state.Gamepad.sThumbLY);
	axes_[2].SetValue(state.Gamepad.sThumbRX);
	axes_[3].SetValue(state.Gamepad.sThumbRY);

	triggers_[0].SetValue(state.Gamepad.bRightTrigger);
	triggers_[1].SetValue(state.Gamepad.bLeftTrigger);

	hatSwitches_[0].SetButton(InputHatSwitchObject::HAT_SWITCH_UP, ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0));
	hatSwitches_[0].SetButton(InputHatSwitchObject::HAT_SWITCH_RIGHT, ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0));
	hatSwitches_[0].SetButton(InputHatSwitchObject::HAT_SWITCH_DOWN, ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0));
	hatSwitches_[0].SetButton(InputHatSwitchObject::HAT_SWITCH_LEFT, ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0));
								 
	buttons_[0].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
	buttons_[1].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
	buttons_[2].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
	buttons_[3].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
	buttons_[6].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
	buttons_[7].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
	buttons_[4].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
	buttons_[5].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
	buttons_[8].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
	buttons_[9].SetValue((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
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
		axes_.push_back(InputAxisObject(-0x7FFF, 0x7FFF));

	if(axes_.size() > 4)
		axes_[3].SetInverted(true);
	
	for(int i = 0; i < inputLayout_.nrOfButtons; i++)
		buttons_.push_back(InputButtonObject());

	for(int i = 0; i < inputLayout_.nrOfHatSwitches; i++)
		hatSwitches_.push_back(InputHatSwitchObject());

	for(int i = 0; i < inputLayout_.nrOfTriggers; i++)
		triggers_.push_back(InputTriggerObject(0, 0xFF));
}