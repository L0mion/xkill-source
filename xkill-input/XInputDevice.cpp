#include "XInputDevice.h"
#include <xkill-utilities/LeanWindows.h>

#include <Xinput.h>
#include "InputActions.h"

#include "InputButtonObject.h"
#include "InputAxisObject.h"
#include "InputTriggerObject.h"

#include "InputObjectArray.h"

#include <xkill-utilities/Converter.h>

XInputDevice::XInputDevice() : 
	InputDevice(GUID(), "")
{
	deviceNr_ = -1;
	rightFFMotor_ = 0xFFFF;
	leftFFMotor_ = 0xFFFF;
	forceFeedbackOn_ = false;

	createInputLayout();
	createInputObjectsFromLayout();
}

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
	InputDevice::Update(deltaTime);
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

bool XInputDevice::StopForceFeedback()
{
	XINPUT_VIBRATION vibration;

	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;

	XInputSetState(deviceNr_, &vibration);

	forceFeedbackOn_ = false;

	return forceFeedbackOn_;
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
		axes_[0]->addMapping(InputAction::ACTION_F_WALK_LR);
		axes_[1]->addMapping(InputAction::ACTION_F_WALK_FB);
		axes_[2]->addMapping(InputAction::ACTION_F_LOOK_LR);
		axes_[2]->setUseAcceleration(true);
		axes_[3]->addMapping(InputAction::ACTION_F_LOOK_UD);
		axes_[3]->setUseAcceleration(true);
		axes_[3]->setInverted(true);
	}

	if(buttons_.size() >= 14)
	{
		buttons_[0]->addMapping(InputAction::ACTION_B_PREV_AMMUNITIONTYPE);
		buttons_[1]->addMapping(InputAction::ACTION_B_NEXT_AMMUNITIONTYPE);
		buttons_[2]->addMapping(InputAction::ACTION_B_NEXT_FIRINGMODE);
		buttons_[3]->addMapping(InputAction::ACTION_B_PREV_FIRINGMODE);
		buttons_[4]->addMapping(InputAction::ACTION_B_JUMP_JETPACK);
		buttons_[5]->addMapping(InputAction::ACTION_B_SPRINT);
		buttons_[7]->addMapping(InputAction::ACTION_B_KILL_PLAYER);
		buttons_[9]->addMapping(InputAction::ACTION_B_RELOAD);
		buttons_[10]->addMapping(InputAction::ACTION_B_PREV_FIRINGMODE);
		buttons_[11]->addMapping(InputAction::ACTION_B_NEXT_AMMUNITIONTYPE);
		buttons_[12]->addMapping(InputAction::ACTION_B_NEXT_FIRINGMODE);
		buttons_[13]->addMapping(InputAction::ACTION_B_PREV_AMMUNITIONTYPE);
	}

	if(triggers_.size() >= 2)
	{
		triggers_[0]->addMapping(InputAction::ACTION_B_FIRE);
		triggers_[1]->addMapping(InputAction::ACTION_F_LOW_SENSITIVITY);
	}
}

InputDevice::InputDeviceType XInputDevice::GetType()
{
	return InputDevice::InputDeviceType::XINPUT_DEVICE;
}

std::string XInputDevice::getStandardMappingsString()
{
	XInputDevice xiDevice;

	xiDevice.setStandardMappings();
	std::vector<InputObject*> inputObjects = xiDevice.inputObjectArray_->inputObjects;

	std::string str = "";

	for(unsigned int i = 0; i < inputObjects.size(); i++)
	{
		std::vector<int>* mappings = inputObjects.at(i)->getMappings();

		for(unsigned int j = 0; j < mappings->size(); j++)
		{
			str += Converter::IntToStr(mappings->at(j));
		}
	}

	return str;
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
		inputObjectArray_->inputObjects.push_back(axis);
	}

	if(axes_.size() >= 4)
	{
		axes_[0]->setName("Left Stick X");
		axes_[1]->setName("Left Stick Y");
		axes_[2]->setName("Right Stick X");
		axes_[3]->setName("Right Stick Y");
	}
	
	for(int i = 0; i < inputLayout_.nrOfButtons + inputLayout_.nrOfHatSwitches*4; i++)
	{
		InputButtonObject* button = new InputButtonObject(i);
		buttons_.push_back(button);
		inputObjectArray_->inputObjects.push_back(button);
	}

	if(buttons_.size() >= 14)
	{
		buttons_[0]->setName("A");
		buttons_[1]->setName("B");
		buttons_[2]->setName("Y");
		buttons_[3]->setName("X");
		buttons_[4]->setName("Left Shoulder");
		buttons_[5]->setName("Right Shoulder");
		buttons_[6]->setName("Start");
		buttons_[7]->setName("Select");
		buttons_[8]->setName("Left Stick Button");
		buttons_[9]->setName("Right Stick Button");
		buttons_[10]->setName("DPad Up");
		buttons_[11]->setName("DPad Right");
		buttons_[12]->setName("DPad Down");
		buttons_[13]->setName("DPad Left");
	}

	for(int i = 0; i < inputLayout_.nrOfTriggers; i++)
	{
		InputTriggerObject* trigger = new InputTriggerObject(0, 0xFF);
		triggers_.push_back(trigger);
		inputObjectArray_->inputObjects.push_back(trigger);
	}

	if(triggers_.size() >= 2)
	{
		triggers_[0]->setName("Right Trigger");
		triggers_[1]->setName("Left Trigger");
	}
}