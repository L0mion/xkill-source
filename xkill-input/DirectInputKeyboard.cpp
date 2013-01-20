#include "DirectInputKeyboard.h"

#include "InputButtonObject.h"
#include "InputAxisObject.h"
#include "InputTriggerObject.h"

DirectInputKeyboard::DirectInputKeyboard(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name, unsigned int playerID) : 
	DirectInputDevice(device, deviceGUID, name, playerID)
{
	hasFF_ = false;
}

DirectInputKeyboard::~DirectInputKeyboard(void)
{
}

bool DirectInputKeyboard::Init(HWND hWindow)
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

	result = device_->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
		return false;

	createInputLayout();
	createInputObjectsFromLayout(&enumObjectsStruct);

	return true;
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

	ZeroMemory(&keyState, sizeof(keyState));

	result = device_->Acquire();
	if(SUCCEEDED(result))
	{
		result = device_->GetDeviceState(sizeof(keyState), &keyState);

		for(int i = 0; !(i >= 256 || i >= inputLayout_.nrOfButtons); i++)
		{
			buttons_[i]->SetValue(keyState[i]);
		}
	}
}
