#include "QTInputDevices.h"

QTInputDevices::QTInputDevices(GUID deviceGUID, std::string name, unsigned int playerID) : 
	InputDevice(deviceGUID, name, playerID)
{
	createInputLayout();
	createInputObjectsFromLayout();
}

QTInputDevices::~QTInputDevices()
{

}

void QTInputDevices::Update(float deltaTime)
{
	updateState();
}

InputDevice::InputDeviceType QTInputDevices::GetType()
{
	return QT_INPUT_DEVICE;
}

void QTInputDevices::updateState()
{
	//Will be done through events in InputComponent
}

void QTInputDevices::createInputLayout()
{
	inputLayout_.nrOfHatSwitches = 0;
	inputLayout_.nrOfTriggers = 0;
	inputLayout_.nrOfButtons = 10;
	inputLayout_.nrOfAxes = 4;
}

void QTInputDevices::createInputObjectsFromLayout()
{
	for(int i = 0; i < inputLayout_.nrOfAxes; i++)
		axes_.push_back(InputAxisObject(-0x7FFF, 0x7FFF));
	
	for(int i = 0; i < inputLayout_.nrOfButtons; i++)
		buttons_.push_back(InputButtonObject());

	if(buttons_.size() >= 5)
	{
		buttons_[3].setKey('W');
		buttons_[4].setKey('A');
		buttons_[5].setKey('S');
		buttons_[6].setKey('D');
		buttons_[0].setKey(0x20); //Space
	}
}

void QTInputDevices::setAxis(unsigned int index, float value)
{
	if(axes_.size() > index)
	{
		axes_[index].AddValue(value);
	}
}

void QTInputDevices::setButton(char key, bool value)
{
	for(unsigned int i = 0; i < buttons_.size(); i++)
	{
		if(buttons_[i].getKey() == key)
		{
			buttons_[i].SetValue(value);
		}
	}
}

void QTInputDevices::setAxesToZero()
{
	for(unsigned int i = 0; i < axes_.size(); i++)
	{
		axes_[i].SetValue(0.0f);
	}
}
  
void QTInputDevices::RunForceFeedback()
{

}
void QTInputDevices::StopForceFeedback()
{

}
void QTInputDevices::SetForceFeedback(float leftMotorScale, float rightMotorScale)
{

}
bool QTInputDevices::IsForceFeedbackCapable()
{
	return false;
}