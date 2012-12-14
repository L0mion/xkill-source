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

class KeyMapper
{
public:
	KeyMapper();
	~KeyMapper();

	bool init(std::string configFilePath);

	void saveConfig(std::vector<InputDevice*> devices);
	void loadConfig(std::vector<InputDevice*> devices);

private:
	FileParser* fileParser_;

	bool isGUIDRow(std::string row, GUID* guid);
	void extractSettingsFromRow(std::string row, InputDevice* device);
	bool handleInputObjectSettings(std::string configSnippet, InputDevice* device);

	void handleAxisObjectSettings(InputAxisObject* axis, std::string settingsString);
	void handleButtonObjectSettings(InputButtonObject* button, std::string settingsString);
	void handleHatSwitchObjectSettings(InputHatSwitchObject* hatSwitch, std::string settingsString);
	void handleTriggerObjectSettings(InputTriggerObject* trigger, std::string settingsString);

	std::string getGUIDstring(GUID* guid);
	std::string getAxesString(std::vector<InputAxisObject*>* axes);
	std::string getButtonString(std::vector<InputButtonObject*>* buttons);
	std::string getHatSwitchString(std::vector<InputHatSwitchObject*>* hatSwitches);
	std::string getTriggerString(std::vector<InputTriggerObject*>* triggers);

	std::string removeComment(std::string str);
	std::string configMessage();
};