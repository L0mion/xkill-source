#include "InputManager.h"

InputManager::InputManager(void)
{
}

InputManager::~InputManager(void)
{
	std::vector<InputDevice*>::iterator it = devices_.begin();
	for(; it != devices_.end(); it++)
		delete (*it);
}

bool InputManager::InitInput(HWND hWindow)
{
	HRESULT result;
	nrOfXInputDevices_ = 0;

	LONG longInstance = GetWindowLong(hWindow, GWL_HINSTANCE);
	HINSTANCE hInstance = (HINSTANCE)(longInstance);

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, NULL);
	if(FAILED(result))
		return false;

	UpdateNumberOfGamepads(hWindow);

	//LPDIRECTINPUTDEVICE8 dInputDevice;
	//result = dInput_->CreateDevice(GUID_SysMouse, &dInputDevice, NULL);
	//if(FAILED(result))
	//	return false;

	//DirectInputDevice* device = new DirectInputMouse(dInputDevice, GUID_SysMouse, "Mouse"); //Kolla ifall musen är inkopplad genom att köra en enum med guid:et
	//device->Init(hWindow);
	//devices_.push_back(device);

	//result = dInput_->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL);
	//if(FAILED(result))
	//	return false;

	//device = new DirectInputKeyboard(dInputDevice, GUID_SysKeyboard, "Keyboard"); //Kolla ifall tangentbordet är inkopplad genom att köra en enum med guid:et
	//device->Init(hWindow);
	//devices_.push_back(device);

	return true;
}

void InputManager::Update(float deltaTime)
{
	for(unsigned int i = 0; i < devices_.size(); i++)
		devices_[i]->Update(deltaTime);

	handleInput();
}

void InputManager::FillAttribute(InputAttribute& inputAttribute, unsigned int deviceNr, float deltaTime)
{
	if(deviceNr >= devices_.size() || deviceNr < 0)
		return;

	InputDevice::InputState state = devices_[deviceNr]->GetState();

	int nrAxes = state.axes.size();
	if(nrAxes >= 1)
		inputAttribute.position[0] += state.axes[0].GetValue() * deltaTime;

	if(nrAxes >= 2)
		inputAttribute.position[1] += state.axes[1].GetValue() * deltaTime;

	if(nrAxes >= 3)
		inputAttribute.rotation[0] += state.axes[2].GetValue() * deltaTime;

	if(nrAxes >= 4)
		inputAttribute.rotation[1] += state.axes[3].GetValue() * deltaTime;
}

int InputManager::UpdateNumberOfGamepads(HWND hWindow)
{
	int nrOfGamepadsAdded = 0;

	EnumDevicesStruct enumDevicesStruct;
	HRESULT result = dInput_->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, &enumDevicesStruct, DIEDFL_ATTACHEDONLY);
	if(FAILED(result))
		return nrOfGamepadsAdded;

	for(unsigned int i = 0; i < enumDevicesStruct.deviceGUIDs.size(); i++)
	{
		bool deviceAlreadyAdded = false;

		for(unsigned int j = 0; j < devices_.size(); j++)
		{
			if(devices_[j]->GetGUID() == enumDevicesStruct.deviceGUIDs[i])
			{
				deviceAlreadyAdded = true;
				break;
			}
		}

		if(!deviceAlreadyAdded)
		{
			addNewDevice(hWindow, enumDevicesStruct.deviceGUIDs[i], enumDevicesStruct.deviceNames[i]);
			nrOfGamepadsAdded++;
		}
	}

	return nrOfGamepadsAdded;
}

bool InputManager::addNewDevice(HWND hWindow, GUID guid, std::string name)
{
	bool deviceAdded = false;

	if(nrOfXInputDevices_ >= 4 || !isXInputDevice(&guid))
	{
		LPDIRECTINPUTDEVICE8 dInputDevice;
		HRESULT result = dInput_->CreateDevice(guid, &dInputDevice, NULL);
		if(FAILED(result))
			return DIENUM_CONTINUE;

		DirectInputDevice* device = new DirectInputDevice(dInputDevice, guid, name);
		device->Init(hWindow);
		devices_.push_back(device);

		deviceAdded = true;
	}
	else
	{
		InputDevice* device = new XInputDevice(nrOfXInputDevices_++, guid, name);
		devices_.push_back(device);

		deviceAdded = true;
	}

	return deviceAdded;
}

void InputManager::handleInput()
{
	InputDevice::InputState inputState;

	for(unsigned int i = 0; i < devices_.size(); i++)
	{
		inputState = devices_[i]->GetState();

		if(inputState.buttons.size() >= 2)
		{
			if(inputState.buttons[0].isReleased())
			{
				devices_[i]->RunForceFeedback();
			}
			else if(inputState.buttons[1].isReleased())
			{
				devices_[i]->StopForceFeedback();
			}
		}

		if(devices_[i]->IsForceFeedbackCapable())
		{
			if(inputState.axes.size() >= 4)
				devices_[i]->SetForceFeedback(std::abs(inputState.axes[2].GetValue()), std::abs(inputState.axes[3].GetValue()));
		}
	}
}

BOOL CALLBACK InputManager::EnumDevicesCallback(const DIDEVICEINSTANCE* device, LPVOID pvRef)
{
	EnumDevicesStruct* eds = (EnumDevicesStruct*)pvRef;

	eds->deviceGUIDs.push_back(device->guidProduct);
	eds->deviceNames.push_back(device->tszProductName);

	return DIENUM_CONTINUE;
}

std::string InputManager::GetInputInformationString()
{
	InputDevice::InputState inputState;
	std::string str = "";

	for(unsigned int i = 0; i < devices_.size(); i++)
	{
		devices_[i]->Update(0.0f);
		inputState = devices_[i]->GetState();

		str += "Device #" + Converter::IntToStr(i) + "\n";
		str += "[ " + devices_[i]->GetName() + " ]\n";

		for(unsigned int j = 0; j < inputState.axes.size(); j++)
		{
			str += "Axis #" + Converter::IntToStr(j) + ": " + Converter::FloatToStr(inputState.axes[j].GetValue()) + "\n";
		}

		for(unsigned int j = 0; j < inputState.triggers.size(); j++)
		{
			str += "Trigger #" + Converter::IntToStr(j) + ": " + Converter::FloatToStr(inputState.triggers[j].GetValue()) + " ";

			if(inputState.triggers[j].IsTriggered())
			{
				str += "On\n";
			}
			else
			{
				str += "Off\n";
			}
		}

		for(unsigned int j = 0; j < inputState.hatSwitches.size(); j++)
		{
			str += "Hat Switch #" + Converter::IntToStr(j) + ": ";

			for(int i = 0; i < 4; i++)
				str += Converter::IntToStr(inputState.hatSwitches[j].buttonDown(i));

			str += "\n";
		}

		for(unsigned int j = 0; j < inputState.buttons.size(); j++)
		{
			str += "Button #" + Converter::IntToStr(j) + ": ";

			if(inputState.buttons[j].isDown())
			{
				str += "On\n";
			}
			else
			{
				str += "Off\n";
			}
		}

		str += "\n";
	}

	return str;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code from msdn: http://msdn.microsoft.com/en-us/library/windows/desktop/ee417014%28v=vs.85%29.aspx 	//
//-------------------------------------------------------------------------------------------------- //
// Enum each PNP device using WMI and check each device ID to see if it contains 						//
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device						//
// Unfortunately this information can not be found by just using DirectInput 							//
//-------------------------------------------------------------------------------------------------- //
BOOL InputManager::isXInputDevice( const GUID* pGuidProductFromDirectInput )							//
{																										//
    IWbemLocator*           pIWbemLocator  = NULL;														//
    IEnumWbemClassObject*   pEnumDevices   = NULL;														//
    IWbemClassObject*       pDevices[20]   = {0};														//
    IWbemServices*          pIWbemServices = NULL;														//
    BSTR                    bstrNamespace  = NULL;														//
    BSTR                    bstrDeviceID   = NULL;														//
    BSTR                    bstrClassName  = NULL;														//
    DWORD                   uReturned      = 0;															//
    bool                    bIsXinputDevice= false;														//
    UINT                    iDevice        = 0;															//
    VARIANT                 var;																		//
    HRESULT                 hr;																			//
																										//
    // CoInit if needed																					//
    hr = CoInitialize(NULL);																			//
    bool bCleanupCOM = SUCCEEDED(hr);																	//
																										//
    // Create WMI																						//
    hr = CoCreateInstance( __uuidof(WbemLocator),														//
                           NULL,																		//
                           CLSCTX_INPROC_SERVER,														//
                           __uuidof(IWbemLocator),														//
                           (LPVOID*) &pIWbemLocator);													//
    if( FAILED(hr) || pIWbemLocator == NULL )															//
        goto LCleanup;																					//
																										//
    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if( bstrNamespace == NULL ) goto LCleanup;  //      
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if( bstrClassName == NULL ) goto LCleanup;  //      
    bstrDeviceID  = SysAllocString( L"DeviceID" );          if( bstrDeviceID == NULL )  goto LCleanup;  //      
    																									//
    // Connect to WMI 																					//
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 									//
                                       0L, NULL, NULL, &pIWbemServices );								//
    if( FAILED(hr) || pIWbemServices == NULL )															//
        goto LCleanup;																					//
																										//
    // Switch security level to IMPERSONATE. 															//
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 						//
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );          //          
																										//
    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 					//
    if( FAILED(hr) || pEnumDevices == NULL )															//
        goto LCleanup;																					//
																										//
    // Loop over all devices																			//
    for( ;; )																							//
    {																									//
        // Get 20 at a time																				//
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );										//
        if( FAILED(hr) )																				//
            goto LCleanup;																				//
        if( uReturned == 0 )																			//
            break;																						//
																										//
        for( iDevice=0; iDevice<uReturned; iDevice++ )													//
        {																								//
            // For each device, get its device ID														//
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );							//
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )							//
            {																							//
                // Check if the device ID contains "IG_".  If it does, then it's an XInput device		//
				    // This information can not be found from DirectInput 								//
                if( wcsstr( var.bstrVal, L"IG_" ) )														//
                {																						//
                    // If it does, then get the VID/PID from var.bstrVal								//
                    DWORD dwPid = 0, dwVid = 0;															//
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );										//
                    if( strVid && swscanf_s( strVid, L"VID_%4X", &dwVid ) != 1 )						//
                        dwVid = 0;																		//
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );										//
                    if( strPid && swscanf_s( strPid, L"PID_%4X", &dwPid ) != 1 )						//
                        dwPid = 0;																		//
																										//
                    // Compare the VID/PID to the DInput device											//
                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );											//
                    if( dwVidPid == pGuidProductFromDirectInput->Data1 )								//
                    {																					//
                        bIsXinputDevice = true;															//
                        goto LCleanup;																	//
                    }																					//
                }																						//
            }   																						//
            SAFE_RELEASE( pDevices[iDevice] );															//
        }																								//
    }																									//
																										//
	LCleanup:																							//
		if(bstrNamespace)																				//
			SysFreeString(bstrNamespace);																//
		if(bstrDeviceID)																				//
			SysFreeString(bstrDeviceID);																//
		if(bstrClassName)																				//
			SysFreeString(bstrClassName);																//
		for( iDevice=0; iDevice<20; iDevice++ )															//
			SAFE_RELEASE( pDevices[iDevice] );															//
		SAFE_RELEASE( pEnumDevices );																	//
		SAFE_RELEASE( pIWbemLocator );																	//
		SAFE_RELEASE( pIWbemServices );																	//
																										//
		if( bCleanupCOM )																				//
			CoUninitialize();																			//
																										//
    return bIsXinputDevice;																				//
}																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////