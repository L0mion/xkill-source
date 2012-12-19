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

	virtual InputObjectType GetType() = 0;

	virtual float getValueFloat() = 0;
	virtual bool getValueBool() = 0;
	virtual bool getValueBoolReleased() = 0;

	void setInverted(bool inverted);
	bool isInverted();

	void addFloatMapping(int mapping);
	void addBoolMapping(int mapping);
	void removeFloatMapping(int mapping);
	void removeBoolMapping(int mapping);

	bool hasFloatMapping(int mapping);
	bool hasBoolMapping(int mapping);
	std::vector<int> getFloatMappings();
	std::vector<int> getBoolMappings();

protected:
	std::vector<int> floatMappings_;
	std::vector<int> boolMappings_;

	bool inverted_;

	void removeMapping(std::vector<int>* vector, int mapping);
	bool hasMapping(std::vector<int>* vector, int mapping);

	void swap(std::vector<int>* vector, int firstIndex, int secondIndex);
};

