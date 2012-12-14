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
	inputLayout_.nrOfAxes = 4;
}

void QTInputDevices::createInputObjectsFromLayout()
{
	for(int i = 0; i < inputLayout_.nrOfAxes; i++)
	{
		axes_.push_back(new InputAxisObject(-0x7FFF, 0x7FFF));
		inputObjects_.push_back(axes_[axes_.size() - 1]);
	}
	
	for(int i = 0; i < 5; i++)
	{
		buttons_.push_back(new InputButtonObject(i));
		inputObjects_.push_back(buttons_[buttons_.size() - 1]);
	}

	if(buttons_.size() >= 5)
	{
		buttons_[0]->setKey('W');
		buttons_[1]->setKey('A');
		buttons_[2]->setKey('S');
		buttons_[3]->setKey('D');
		buttons_[4]->setKey(0x20); //Space
	}
}

void QTInputDevices::setAxis(unsigned int index, float value)
{
	if(axes_.size() > index)
	{
		axes_[index]->AddValue(value);
	}
}

void QTInputDevices::setButton(char key, bool value)
{
	for(unsigned int i = 0; i < buttons_.size(); i++)
	{
		if(buttons_[i]->getKey() == key)
		{
			buttons_[i]->SetValue(value);
		}
	}
}

void QTInputDevices::setAxesToZero()
{
	for(unsigned int i = 0; i < axes_.size(); i++)
	{
		axes_[i]->SetValue(0.0f);
	}
}

void QTInputDevices::updateButtons()
{
	for(unsigned int i = 0; i < buttons_.size(); i++)
		buttons_[i]->SetValue(buttons_[i]->isDown());
}

InputButtonObject* QTInputDevices::getButtonObject(int index)
{
	InputButtonObject* button = nullptr;

	for(unsigned int i = 0; i < buttons_.size(); i++)
	{
		if(buttons_[i]->getKey() == (char)index)
		{
			button = buttons_[i];
			break;
		}
	}

	if(button == nullptr)
	{
		button = new InputButtonObject(index);
		buttons_.push_back(button);
		inputObjects_.push_back(button);
		inputLayout_.nrOfButtons++;
	}

	return button;
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