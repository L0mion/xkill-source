#pragma once

#include <string>
#include <vector>

#include <InitGuid.h>

class InputDevice;
class InputAxisObject;
class InputButtonObject;
class InputTriggerObject;
class InputHatSwitchObject;
class FileParser;

//! Used to parse and save keymap files
/*!
Uses a ".gli" file to keep track of which files refers to which device.
Uses a ".kmp" file to save device settings

Uses plain text. Should perhaps use binary files to speed up mid game controller connection.
*/

class KeyMapper
{
public:
	KeyMapper();
	~KeyMapper();

	//! Inits file parser and loads file list
	bool init(std::string configFilePath);

	//! Save the configs of all devices in the vector
	void saveConfig(std::vector<InputDevice*> devices);
	//! Loads the configs to all devices in the vector
	void loadConfig(std::vector<InputDevice*> devices);
	//! Loads the config of the device
	void getConfigForNewController(InputDevice* device);

private:
	//! Used to handle the files
	FileParser* fileParser_;
	//! Stores the connections between guid and ".kmp" file
	std::vector<std::pair<GUID, std::string>> fileList_;

	//! Load the file list
	void parseFileList();
	//! Stores the file list
	void writeFileList();

	//! Checks if a row has a guid setting and extracts and returns the data
	bool isGUIDRow(std::string row, GUID& guid, std::string& fileName);

	// Handle file reading
	//! Parses a single ".kmp" file
	bool parseFile(InputDevice* device);
	//! Extracts all settings from a row and sets corresponding setting in device
	void extractSettingsFromRow(std::string row, InputDevice* device);
	//! Handles the settings of an single input object
	bool handleInputObjectSettings(std::string configSnippet, InputDevice* device);

	//! Handles axis object settings
	void handleAxisObjectSettings(InputAxisObject* axis, std::string settingsString);
	//! Handles button object settings
	void handleButtonObjectSettings(InputButtonObject* button, std::string settingsString);
	//! Handles trigger objects settings
	void handleTriggerObjectSettings(InputTriggerObject* trigger, std::string settingsString);

	//! Splits a settingsstring into a vector of settings
	std::vector<std::pair<char, std::string>>* settingsExtracter(std::string settingsString);

	//Handle file writing
	//! Write the configs of a device to corresponding 
	void writeToFile(InputDevice* device);
	//! Returns a string constructed from a guid
	std::string getGUIDstring(GUID* guid);
	//! Returns a string constructed from all the settings of the axis objects in the vector
	std::string getAxesString(std::vector<InputAxisObject*>* axes);
	//! Returns a string constructed from all the settings of the button objects in the vector
	std::string getButtonString(std::vector<InputButtonObject*>* buttons);
	//! Returns a string constructed from all the settings of the trigger objects in the vector
	std::string getTriggerString(std::vector<InputTriggerObject*>* triggers);

	//! Removes comments from the string
	std::string removeComment(std::string str);
};