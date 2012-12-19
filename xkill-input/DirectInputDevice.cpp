#include "DirectInputDevice.h"


DirectInputDevice::DirectInputDevice(LPDIRECTINPUTDEVICE8 device, GUID deviceGUID, std::string name, unsigned int playerID) : 
	InputDevice(deviceGUID, name, playerID)
{
	device_ = device;
	hasFF_ = false;
	isFFTurnedOn_ = false;
}

DirectInputDevice::~DirectInputDevice(void)
{
	if(hasFF_)
	{
		StopForceFeedback();
		SAFE_RELEASE(effect_);
	}
	device_->Unacquire();
	SAFE_RELEASE(device_);
}

void DirectInputDevice::RunForceFeedback()
{
	if(hasFF_)
	{
		if(!isFFTurnedOn_)
		{
			HRESULT result = effect_->Start(INFINITE, NULL);

			if(SUCCEEDED(result))
				isFFTurnedOn_ = true;
		}
	}
}

void DirectInputDevice::StopForceFeedback()
{
	if(hasFF_)
	{
		if(isFFTurnedOn_)
		{
			if(SUCCEEDED(effect_->Stop()))
				isFFTurnedOn_ = false;
		}
	}
}

void DirectInputDevice::SetForceFeedback(float leftMotorScale, float rightMotorScale)
{
	if(!hasFF_ || !isFFTurnedOn_)
		return;

	rightFFMotorScale_ = rightMotorScale;
	leftFFMotorScale_ = leftMotorScale;

	LONG rglDirection[2] = {0, 0};
	DICONSTANTFORCE cf;

    if( nrOfFFMotors_ == 1 )
    {
		cf.lMagnitude = (LONG)((rightFFMotorScale_ * DI_FFNOMINALMAX + 
			leftFFMotorScale_ * DI_FFNOMINALMAX)/2.0);
        rglDirection[0] = 0;
    }
    else
    {
        rglDirection[0] = (LONG)(rightFFMotorScale_ * DI_FFNOMINALMAX);
        rglDirection[1] = (LONG)(leftFFMotorScale_ * DI_FFNOMINALMAX);
        cf.lMagnitude = (DWORD)sqrt((double)rglDirection[0] * (double)rglDirection[0] + (double)rglDirection[1] * (double)rglDirection[1]);
    }

    DIEFFECT effect;
    ZeroMemory(&effect, sizeof(effect));
    effect.dwSize = sizeof(DIEFFECT);
    effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    effect.cAxes = nrOfFFMotors_;
    effect.rglDirection = rglDirection;
    effect.lpEnvelope = 0;
    effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
    effect.lpvTypeSpecificParams = &cf;
    effect.dwStartDelay = 0;
	
	if(isFFTurnedOn_)
	{
		if(SUCCEEDED(effect_->SetParameters(&effect, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START )))
		{
			isFFTurnedOn_ = true;
		}
	}
	else
	{
		effect_->SetParameters(&effect, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS);
	}
}

bool DirectInputDevice::IsForceFeedbackCapable()
{
	return hasFF_;
}

void DirectInputDevice::Update(float deltaTime)
{
	updateState();
}

bool DirectInputDevice::Init(HWND hWindow)
{
	HRESULT result;

	EnumObjectsStruct enumObjectsStruct;
	//enumObjectsStruct.device = device_;
	result = device_->EnumObjects(EnumObjectsCallback, &enumObjectsStruct, DIDFT_ALL);
	if(FAILED(result))
		return false;

	hasFF_ = (enumObjectsStruct.nrOfFFObjects > 0);

	result = device_->SetCooperativeLevel(hWindow, DISCL_BACKGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
		return false;

	result = device_->SetDataFormat(&c_dfDIJoystick);
	if(FAILED(result))
		return false;

	if(hasFF_)
	{
		DIPROPDWORD DIPropAutoCenter;

		DIPropAutoCenter.diph.dwSize = sizeof(DIPropAutoCenter);
		DIPropAutoCenter.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		DIPropAutoCenter.diph.dwObj = 0;
		DIPropAutoCenter.diph.dwHow = DIPH_DEVICE;
		DIPropAutoCenter.dwData = DIPROPAUTOCENTER_OFF;

		result = device_->SetProperty(DIPROP_AUTOCENTER, &DIPropAutoCenter.diph);
		if(FAILED(result))
			return false;
	}

	//result = enumDevicesStruct.devices[i]->CreateEffect(GUID_Square, NULL, &(mEffects[i]), NULL);

	result = device_->Acquire(); //If failed to acquire then create effect will also fail
	//if(FAILED(result))
	//	return;

	if(hasFF_)
	{
		nrOfFFMotors_ = enumObjectsStruct.nrOfFFObjects;
		result = createEffect(nrOfFFMotors_);
		if(FAILED(result))
		{
			hasFF_ = false;
		}
	}

	createInputLayout();
	createInputObjectsFromLayout();

	return true;
}

void DirectInputDevice::setStandardMappings()
{
	if(axes_.size() >= 4)
	{
		axes_[0]->addFloatMapping(0);
		axes_[1]->addFloatMapping(1);
		axes_[2]->addFloatMapping(2);
		axes_[3]->addFloatMapping(3);
	}

	if(buttons_.size() >= 10)
	{
		buttons_[9]->addBoolMapping(0);
	}
	else if(buttons_.size() >= 1)
	{
		buttons_[0]->addBoolMapping(0);
	}
}

InputDevice::InputDeviceType DirectInputDevice::GetType()
{
	return InputDevice::InputDeviceType::DIRECT_INPUT_DEVICE;
}

BOOL CALLBACK DirectInputDevice::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* object, LPVOID pvRef)
{
	EnumObjectsStruct* eos = (EnumObjectsStruct*)pvRef;

	if((object->dwFlags & DIDOI_FFACTUATOR) != 0)
	{
		//setup FF object

		eos->nrOfFFObjects++;
	}
	//else if((object->dwType & DIDFT_AXIS) != 0)
	//{
	//	//SetupAxisObject(object, eos->device);
	//	eos->offset += sizeof(long);
	//	eos->ObjectDataFormat.push_back(CreateDataFormat(object, eos->ObjectDataFormat.size()));
	//}
	//else if((object->dwType & DIDFT_BUTTON) != 0)
	//{
	//	//Setup button object
	//	eos->offset += sizeof(byte);
	//	eos->ObjectDataFormat.push_back(CreateDataFormat(object, eos->ObjectDataFormat.size()));
	//}
	//else if((object->dwType & DIDFT_POV) != 0)
	//{
	//	eos->offset += sizeof(long);
	//	eos->ObjectDataFormat.push_back(CreateDataFormat(object, eos->ObjectDataFormat.size()));
	//}

	return DIENUM_CONTINUE;
}

//DIOBJECTDATAFORMAT DirectInputDevice::CreateDataFormat(const DIDEVICEOBJECTINSTANCE* object, int objectNumber)
//{
//	DIOBJECTDATAFORMAT objectDataFormat;
//
//	objectDataFormat.pguid = &object->guidType;
//	objectDataFormat.dwType = object->dwType | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL;
//	objectDataFormat.dwOfs = object->dwOfs;
//	objectDataFormat.dwFlags = 0;
//
//	return objectDataFormat;
//}
//
//BOOL DirectInputDevice::SetupAxisObject(const DIDEVICEOBJECTINSTANCE* object, LPDIRECTINPUTDEVICE8 device)
//{
//	HRESULT result;
//
//	DIPROPRANGE propRange;
//	DIPROPDWORD propWord;
//
//	propRange.diph.dwSize = sizeof(DIPROPRANGE);
//	propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
//	propRange.diph.dwHow = DIPH_BYID;
//	propRange.diph.dwObj = object->dwType;
//
//	propRange.lMin = -1000;
//	propRange.lMin = +1000;
//
//	result = device->SetProperty(DIPROP_RANGE, &propRange.diph);
//	if(FAILED(result))
//		return false;
//
//	propWord.diph.dwSize = sizeof(DIPROPDWORD);
//	propWord.diph.dwHeaderSize = sizeof(DIPROPHEADER);
//	propWord.diph.dwHow = DIPH_BYID;
//	propWord.diph.dwObj = object->dwType;
//
//	propWord.dwData = 4000;
//
//	result = device->SetProperty(DIPROP_DEADZONE, &propWord.diph);
//	if(FAILED(result))
//		return false;
//
//	return true;
//}

HRESULT DirectInputDevice::createEffect(int nrFFObjects)
{
	DWORD rgdwAxes[2] = {DIJOFS_X, DIJOFS_Y};
	LONG rglDirection[2] = {DI_FFNOMINALMAX, DI_FFNOMINALMAX};
	DICONSTANTFORCE cf;
	cf.lMagnitude = DI_FFNOMINALMAX;

	DIEFFECT effect;

	ZeroMemory( &effect, sizeof( effect ) );
	effect.dwSize = sizeof( DIEFFECT );
	effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	effect.dwDuration = INFINITE;
	effect.dwSamplePeriod = 0;
	effect.dwGain = DI_FFNOMINALMAX;
	effect.dwTriggerButton = DIEB_NOTRIGGER;
	effect.dwTriggerRepeatInterval = 0;
	effect.cAxes = nrFFObjects;
	effect.rgdwAxes = rgdwAxes;
	effect.rglDirection = rglDirection;
	effect.lpEnvelope = 0;
	effect.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
	effect.lpvTypeSpecificParams = &cf;
	effect.dwStartDelay = 0;

	return device_->CreateEffect(GUID_ConstantForce, &effect, &effect_, NULL);
}

//HRESULT DirectInputDevice::setDeviceDataFormat(std::vector<DIOBJECTDATAFORMAT> ODFArray)
//{
//	DIDATAFORMAT dataFormat;
//
//	ZeroMemory(&dataFormat, sizeof(dataFormat));
//
//	dataFormat.dwSize = sizeof(DIDATAFORMAT);
//	dataFormat.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
//	dataFormat.dwFlags = DIDFT_ABSAXIS;
//	dataFormat.dwDataSize = sizeof(InputData);
//	dataFormat.dwNumObjs = ODFArray.size();
//	dataFormat.rgodf = &ODFArray[0];
//
//	return device_->SetDataFormat(&dataFormat);
//}

void DirectInputDevice::updateState()
{
	HRESULT result;
	DIJOYSTATE joyState;
	InputDevice::InputState inputState;

	ZeroMemory(&joyState, sizeof(joyState));

	result = device_->Acquire();
	if(SUCCEEDED(result))
	{
		result = device_->GetDeviceState(sizeof(joyState), &joyState);

		int value = 0;
		for(int i = 0; i < inputLayout_.nrOfAxes; i++)
		{
			switch(axesIndexArray_[i])
			{
				case 0:
				{
					value = joyState.lX;
					break;
				}
				case 1:
				{
					value = joyState.lY;
					break;
				}
				case 2:
				{
					value = joyState.lZ;
					break;
				}
				case 3:
				{
					value = joyState.lRx;
					break;
				}
				case 4:
				{
					value = joyState.lRy;
					break;
				}
				case 5:
				{
					value = joyState.lRz;
					break;
				}
				default:
				{
					value = 0;
					break;
				}
			}

			axes_[i]->SetValue(value);
		}

		for(int i = 0; i < inputLayout_.nrOfButtons; i++)
		{
			buttons_[i]->SetValue(joyState.rgbButtons[i]);
		}

		int HatSwitch;
		int hatSwitchLimit = inputLayout_.nrOfButtons + inputLayout_.nrOfHatSwitches*4;
		int hatSwitchNumber = 0;

		for(int i = inputLayout_.nrOfButtons; i < inputLayout_.nrOfHatSwitches; i += 4)
		{
			HatSwitch = joyState.rgdwPOV[hatSwitchNumber++];

			buttons_[i]->SetValue(HatSwitch == 0		||	HatSwitch == 4500	||	HatSwitch == 31500);
			buttons_[i]->SetValue(HatSwitch == 9000		||	HatSwitch == 13500	||	HatSwitch == 4500);
			buttons_[i]->SetValue(HatSwitch == 18000	||	HatSwitch == 22500	||	HatSwitch == 13500);
			buttons_[i]->SetValue(HatSwitch == 27000	||	HatSwitch == 31500	||	HatSwitch == 22500);
		}
	}
}

void DirectInputDevice::createInputLayout()
{
	ZeroMemory(&inputLayout_, sizeof(inputLayout_));
	DIDEVCAPS deviceCapabilities;
	deviceCapabilities.dwSize = sizeof(DIDEVCAPS);
	HRESULT result = device_->GetCapabilities(&deviceCapabilities);
	if(SUCCEEDED(result))
	{
		inputLayout_.nrOfAxes = deviceCapabilities.dwAxes;
		inputLayout_.nrOfButtons = deviceCapabilities.dwButtons;
		inputLayout_.nrOfHatSwitches = deviceCapabilities.dwPOVs;
		inputLayout_.nrOfTriggers = 0;
	}
}

void DirectInputDevice::createInputObjectsFromLayout()
{
	createAxes();

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

void DirectInputDevice::createAxes()
{
	HRESULT result;
	DIJOYSTATE joyState;
	InputDevice::InputState inputState;

	ZeroMemory(&joyState, sizeof(joyState));

	result = device_->Acquire();
	if(SUCCEEDED(result))
	{
		result = device_->GetDeviceState(sizeof(joyState), &joyState);

		if(joyState.lX != 0) axesIndexArray_.push_back(0);
		if(joyState.lY != 0) axesIndexArray_.push_back(1);
		if(joyState.lZ != 0) axesIndexArray_.push_back(2);
		if(joyState.lRx != 0) axesIndexArray_.push_back(3);
		if(joyState.lRy != 0) axesIndexArray_.push_back(4);
		if(joyState.lRz != 0) axesIndexArray_.push_back(5);
	}

	while(axesIndexArray_.size() > axes_.size())
	{
		axes_.push_back(new InputAxisObject(0, 0xFFFF));
		inputObjects_.push_back(axes_[axes_.size() - 1]);
	}

	if(axesIndexArray_.size() >= 2)
	{
		axes_[1]->setInverted(true);
	}

	inputLayout_.nrOfAxes = axes_.size();
}