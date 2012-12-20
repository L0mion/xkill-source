#include "QTInputDevices.h"

#include "InputActions.h"

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

void QTInputDevices::setStandardMappings()
{
	if(axes_.size() >= 2)
	{
		axes_[0]->addFloatMapping(ACTION_F_LOOK_LR);
		axes_[0]->setDeadZone(0.0f);
		axes_[1]->addFloatMapping(ACTION_F_LOOK_UD);
		axes_[1]->setDeadZone(0.0f);
	}

	if(axes_.size() >= 4)
	{
		axes_[2]->addFloatMapping(ACTION_F_LOOK_LR);
		axes_[2]->setDeadZone(0.0f);
		axes_[3]->addFloatMapping(ACTION_F_LOOK_UD);
		axes_[3]->setDeadZone(0.0f);
	}

	if(buttons_.size() >= inputLayout_.nrOfButtons)
	{
		buttons_[0]->addBoolMapping(ACTION_B_WALK_FORWARD);
		buttons_[1]->addBoolMapping(ACTION_B_WALK_LEFT);
		buttons_[2]->addBoolMapping(ACTION_B_WALK_BACKWARD);
		buttons_[3]->addBoolMapping(ACTION_B_WALK_RIGHT);
		buttons_[4]->addBoolMapping(ACTION_B_FIRE);
		buttons_[5]->addBoolMapping(ACTION_B_CHANGE_AMMUNITIONTYPE);
		buttons_[6]->addBoolMapping(ACTION_B_CHANGE_FIRINGMODE);
	}
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
	inputLayout_.nrOfButtons = 7;
	inputLayout_.nrOfTriggers = 0;
	inputLayout_.nrOfAxes = 4;
}

void QTInputDevices::createInputObjectsFromLayout()
{
	for(int i = 0; i < inputLayout_.nrOfAxes; i++)
	{
		InputAxisObject* axis = new InputAxisObject(-0x7FFF, 0x7FFF);
		axes_.push_back(axis);
		inputObjects_.push_back(axis);
	}
	
	for(int i = 0; i < inputLayout_.nrOfButtons; i++)
	{
		InputButtonObject* button = new InputButtonObject(i);
		buttons_.push_back(button);
		inputObjects_.push_back(button);
	}

	if(buttons_.size() >= inputLayout_.nrOfButtons)
	{
		buttons_[0]->setKey('W');
		buttons_[1]->setKey('A');
		buttons_[2]->setKey('S');
		buttons_[3]->setKey('D');
		buttons_[4]->setKey(0x20); //Space
		buttons_[5]->setKey('Q');
		buttons_[6]->setKey('E');
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

InputButtonObject* QTInputDevices::getButtonObject(unsigned int index)
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