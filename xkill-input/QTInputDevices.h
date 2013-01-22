#pragma once

#include "InputDevice.h"

class InputObject;
class InputAxisObject;
class InputButtonObject;
class InputTriggerObject;

class QTInputDevices : public InputDevice
{
	
	QTInputDevices();
public:
	QTInputDevices(GUID deviceGUID, std::string name, unsigned int playerID = -1);
	~QTInputDevices();

	virtual void Update(float deltaTime);

	virtual void RunForceFeedback();
	virtual bool StopForceFeedback();
	virtual void SetForceFeedback(float leftMotorScale, float rightMotorScale);
	virtual bool IsForceFeedbackCapable();

	virtual void setStandardMappings();

	virtual InputDeviceType GetType();

	void setAxis(unsigned int index, float value);
	void setButton(char key, bool value);
	void setMouseButton(unsigned int nr, bool value);

	void setAxesToZero();
	void updateButtons();

protected:
	std::vector<InputButtonObject*> mouseButtons_;

	virtual void updateState();
	virtual void createInputLayout();
	virtual void createInputObjectsFromLayout();

	virtual std::string getStandardMappingsString();

	virtual InputButtonObject* getButtonObject(unsigned int index);
};