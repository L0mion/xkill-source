#include "DirectInputMouse.h"

DirectInputMouse::DirectInputMouse(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name, unsigned int playerID) : 
	DirectInputDevice(device, deviceGUID, name, playerID)
{
	hasFF_ = false;
}

DirectInputMouse::~DirectInputMouse(void)
{
}

void DirectInputMouse::Update(float deltaTime)
{
	DirectInputDevice::Update(deltaTime);
}

InputDevice::InputDeviceType DirectInputMouse::GetType()
{
	return DIRECT_INPUT_MOUSE;
}

bool DirectInputMouse::Init(HWND hWindow)
{
	HRESULT result;

	EnumObjectsStruct enumObjectsStruct;
	//enumObjectsStruct.device = device_;
	result = device_->EnumObjects(EnumObjectsCallback, &enumObjectsStruct, DIDFT_ALL);
	if(FAILED(result))
		return false;

	result = device_->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
		return false;

	result = device_->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(result))
		return false;

	createInputLayout();
	createInputObjectsFromLayout();

	return true;
}

void DirectInputMouse::updateState()
{
	HRESULT result;
	DIMOUSESTATE2 mouseState;

	ZeroMemory(&mouseState, sizeof(mouseState));

	result = device_->Acquire();
	if(SUCCEEDED(result))
	{
		result = device_->GetDeviceState(sizeof(mouseState), &mouseState);

		axes_[0]->SetValue(mouseState.lX);
		axes_[1]->SetValue(mouseState.lY);

		if(axes_.size() >= 3)
			axes_[2]->SetValue(mouseState.lZ);

		for(int i = 0; i < inputLayout_.nrOfButtons; i++)
		{
			buttons_[i]->SetValue(mouseState.rgbButtons[i]);
		}
	}
}

void DirectInputMouse::createInputObjectsFromLayout()
{
	for(int i = 0; i < inputLayout_.nrOfAxes; i++)
	{
		InputAxisObject* axis = new InputAxisObject(-0xFF, 0xFF);
		axes_.push_back(axis);
		inputObjects_.push_back(axis);
	}
	
	for(int i = 0; i < inputLayout_.nrOfButtons; i++)
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
