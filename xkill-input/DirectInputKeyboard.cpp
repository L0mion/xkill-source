#include "DirectInputKeyboard.h"

DirectInputKeyboard::DirectInputKeyboard(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name) : 
	DirectInputDevice(device, deviceGUID, name)
{
}

DirectInputKeyboard::~DirectInputKeyboard(void)
{
}

void DirectInputKeyboard::Init(HWND hWindow)
{
	HRESULT result;

	EnumObjectsStruct enumObjectsStruct;
	//enumObjectsStruct.device = device_;
	result = device_->EnumObjects(EnumObjectsCallback, &enumObjectsStruct, DIDFT_ALL);
	if(FAILED(result))
		return;

	result = device_->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
		return;

	result = device_->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
		return;

	createInputLayout();
	createInputObjectsFromLayout();
}

void DirectInputKeyboard::Update(float deltaTime)
{
	DirectInputDevice::Update(deltaTime);
}

InputDevice::InputDeviceType DirectInputKeyboard::GetType()
{
	return DIRECT_INPUT_KEYBOARD;
}

void DirectInputKeyboard::updateState()
{
	HRESULT result;
	byte keyState[256];
	InputDevice::InputState inputState;

	ZeroMemory(&keyState, sizeof(keyState));

	result = device_->Acquire();
	if(SUCCEEDED(result))
	{
		result = device_->GetDeviceState(sizeof(keyState), &keyState);

		for(int i = 0; !(i >= 256 || i >= inputLayout_.nrOfButtons); i++)
		{
			buttons_[i].SetValue(keyState[i]);
		}
	}
}
