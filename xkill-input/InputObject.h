#pragma once

#include <vector>

//! An interface for input objects
/*!
An input object contains state data from an object on the 
device, functions that format the data as well as settings
concerning the object.
*/

class InputObject
{
public:
	//! Used to differentiate between different input objects
	enum InputObjectType
	{
		AXIS_OBJECT,
		BUTTON_OBJECT,
		TRIGGER_OBJECT
	};

	InputObject(void);
	virtual ~InputObject(void);

	virtual void update(float delta);

	virtual InputObjectType GetType() = 0;

	virtual float getValueFloat() = 0;
	virtual bool getValueBool() = 0;
	virtual bool getValueBoolPressed() = 0;
	virtual bool getValueBoolReleased() = 0;

	virtual void setInverted(bool inverted);
	virtual bool isInverted();

	virtual void setSensitivity(float sensitivity);
	virtual float getSensitivity();

	virtual void setUseAcceleration(bool useAcceleration);
	virtual void setIsInAccelerationZone(bool inAccelerationZone);
	virtual void setAccelerationRate(float accelerationRate);
	virtual void setMaxAcceleration(float maxAcceleration);

	virtual bool useAcceleration();
	virtual float getAcceleration();

	virtual void setName(std::string name);
	virtual std::string getName();

	virtual bool needsDelta();

	//! Adds a mapping
	void addMapping(int mapping);
	//! Removes a mapping
	void removeMapping(int mapping);

	//! Returns true if that mapping exists
	bool hasMapping(int mapping);
	//! Returns the floatMappings list
	std::vector<int>* getMappings();

protected:
	std::vector<int> mappings_;
	std::string name_;

	bool inverted_;
	float sensitivity_;

	bool useAcceleration_;
	bool inAccelerationZone_;
	float currentAcceleration_;
	float accelerationRate_;
	float maxAcceleration_;

	void removeMapping(std::vector<int>* vector, int mapping);
	bool hasMapping(std::vector<int>* vector, int mapping);

	void swap(std::vector<int>* vector, int firstIndex, int secondIndex);
};