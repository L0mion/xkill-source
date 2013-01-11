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

	void setSensitivity(float sensitivity);
	float getSensitivity();

	void setName(std::string name);
	std::string getName();

	//! Adds a mapping that needs a float value
	void addFloatMapping(int mapping);
	//! Adds a mapping that needs a bool value
	void addBoolMapping(int mapping);
	//! Removes a mapping from the floatMappings_ list
	void removeFloatMapping(int mapping);
	//! Removes a mapping from the boolMappings_ list
	void removeBoolMapping(int mapping);

	//! Returns true if that mapping exists in the floatMappings_ list
	bool hasFloatMapping(int mapping);
	//! Returns true if that mapping exists in the boolMappings_ list
	bool hasBoolMapping(int mapping);
	//! Returns the floatMappings list
	std::vector<int>* getFloatMappings();
	//! Returns the boolMappings list
	std::vector<int>* getBoolMappings();

protected:
	std::vector<int> floatMappings_;
	std::vector<int> boolMappings_;
	std::string name_;

	bool inverted_;
	float sensitivity_;

	void removeMapping(std::vector<int>* vector, int mapping);
	bool hasMapping(std::vector<int>* vector, int mapping);

	void swap(std::vector<int>* vector, int firstIndex, int secondIndex);
};

