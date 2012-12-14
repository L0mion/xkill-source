#include "KeyMapper.h"

#include "FileParser.h"
#include "InputDevice.h"
#include "Converter.h"

KeyMapper::KeyMapper()
{

}

KeyMapper::~KeyMapper()
{
	delete fileParser_;
}

bool KeyMapper::init(std::string configFilePath)
{
	fileParser_ = new FileParser(configMessage());
	fileParser_->setFilePath(configFilePath);
	fileParser_->setFileName("keymappings.cfg");

	return true;
}

void KeyMapper::saveConfig(std::vector<InputDevice*> devices)
{
	fileParser_->startWriting();

	bool doneWriting = false;
	InputDevice* device = nullptr;
	unsigned int index = 0;

	while(!doneWriting)
	{
		if(device == nullptr)
		{
			if(index >= devices.size())
			{
				doneWriting = true;
			}
			else
			{
				device = devices[index++];
				fileParser_->writeRow(getGUIDstring(&device->GetGUID()));
			}
		}
		else
		{
			std::string row = "";

			row = getAxesString(&device->axes_);
			if(row != "")
				fileParser_->writeRow(row);

			row = getButtonString(&device->buttons_);
			if(row != "")
				fileParser_->writeRow(row);

			row = getHatSwitchString(&device->hatSwitches_);
			if(row != "")
				fileParser_->writeRow(row);

			row = getTriggerString(&device->triggers_);
			if(row != "")
				fileParser_->writeRow(row);

			fileParser_->writeRow("");

			device = nullptr;
		}
	}
	
	fileParser_->doneWriting();
}

void KeyMapper::loadConfig(std::vector<InputDevice*> devices)
{
	fileParser_->startReading();

	GUID guid;
	std::string row;
	InputDevice* device = nullptr;

	while(!fileParser_->isEmpty())
	{
		row = fileParser_->getNextRow();
		row = removeComment(row);

		if(isGUIDRow(row, &guid))
		{
			for(unsigned int i = 0; i < devices.size(); i++)
			{
				if(devices[i]->GetGUID() == guid)
				{
					device = devices[i];
					break;
				}
			}
		}
		else if(device != nullptr)
		{
			extractSettingsFromRow(row, device);
		}
	}
}

bool KeyMapper::isGUIDRow(std::string row, GUID* guid)
{
	int endIndex = row.find_first_of("guid");
	int startIndex = 0;
	if(endIndex == row.npos)
		return false;

	startIndex = endIndex + 5;
	row = row.substr(startIndex);
	endIndex = row.find_first_of('-');
	unsigned int value = 0;
	if(!Converter::StrToUInt(row.substr(0, endIndex), value))
		return false;

	guid->Data1 = value;

	startIndex = endIndex + 1;
	row = row.substr(startIndex);
	endIndex = row.find_first_of('-');
	if(!Converter::StrToUInt(row.substr(0, endIndex), value))
		return false;

	guid->Data2 = value;

	startIndex = endIndex + 1;
	row = row.substr(startIndex);
	endIndex = row.find_first_of('-');
	if(!Converter::StrToUInt(row.substr(0, endIndex), value))
		return false;

	guid->Data3 = value;

	row = row.substr(endIndex + 1);

	for(unsigned int i = 0; i < row.size() && i < 8; i++)
		guid->Data4[i] = row[i];

	return true;
}

void KeyMapper::extractSettingsFromRow(std::string row, InputDevice* device)
{
	int startIndex, endIndex;
	std::string configSnippet;
	bool rowEmpty = (row.size() <= 0);

	while(!rowEmpty)
	{
		startIndex = row.find_first_of('#');
		endIndex = row.find_first_of('#', startIndex + 1);
		configSnippet = row.substr(startIndex, endIndex);

		handleInputObjectSettings(configSnippet, device);

		if(endIndex == row.npos)
		{
			rowEmpty = true;
		}
		else
		{
			row = row.substr(endIndex);
		}
	}
}

bool KeyMapper::handleInputObjectSettings(std::string configSnippet, InputDevice* device)
{
	std::string objectString, settingsString;
	char objectChar;
	unsigned int objectIndex;

	int	strIndex = configSnippet.find_first_of("#");
	if(strIndex == configSnippet.npos)
		return false;

	configSnippet = configSnippet.substr(strIndex + 1);
	strIndex = configSnippet.find_first_of(":");
	if(strIndex == configSnippet.npos)
		return false;

	settingsString = configSnippet.substr(strIndex + 1);
	objectString = configSnippet.substr(0, strIndex);
	if(objectString.size() < 2)
		return false;

	objectChar = objectString[0];
	objectString = objectString.substr(1);
	if(Converter::StrToUInt(objectString, objectIndex) && objectIndex < 0)
		return false;

	if(settingsString.size() <= 0)
		return false;

	InputButtonObject* button = nullptr;

	switch(objectChar)
	{
	case 'A': //Axis object
		if(objectIndex < device->axes_.size())
		{
			handleAxisObjectSettings(device->axes_[objectIndex], settingsString);
		}
		break;
	case 'B': //Button object
		button = device->getButtonObject(objectIndex);
		handleButtonObjectSettings(button, settingsString);
		break;
	case 'H': //Hatswitch object
		if(objectIndex < device->buttons_.size())
		{
			handleHatSwitchObjectSettings(device->hatSwitches_[objectIndex], settingsString);	
		}
		break;
	case 'T': //Trigger object
		if(objectIndex < device->triggers_.size())
		{
			handleTriggerObjectSettings(device->triggers_[objectIndex], settingsString);	
		}
		break;
	default:
		return false;
	};

	return true;
}

void KeyMapper::handleAxisObjectSettings(InputAxisObject* axis, std::string settingsString)
{
	std::string setting;
	char settingChar;
	int settingValue;
	int index;
	float settingFloatValue;

	while(settingsString != "")
	{
		index = settingsString.find_first_of(" ");
		if(index < 0)
			break;

		setting = settingsString.substr(0, index);
		settingsString = settingsString.substr(index + 1);

		if(setting.size() < 3)
			continue;

		settingChar = setting[0];
		index = setting.find_first_of('=');
		if(index == setting.npos)
			continue;

		setting = setting.substr(index + 1);
		settingValue = Converter::StrToInt(setting);
		if(settingValue < 0)
			continue;

		switch(settingChar)
		{
		case 'F': //Float key mapping
			axis->addFloatMapping(settingValue);
			break;
		case 'B': //Bool key mapping
			axis->addBoolMapping(settingValue);
			break;
		case 'D': //Deadzone
			settingFloatValue = Converter::StrToFloat(setting);
			if(settingFloatValue < 0.0f)
				settingFloatValue = 0.0f;
			axis->setDeadZone(settingFloatValue);
			break;
		case 'T': //Trigger value
			//Set trigger value
			break;
		case 'I': //Inverted
			axis->setInverted(settingValue > 0);
			break;
		case 'S':
			//Set sensitivity
			break;
		default:
			break;
		};
	}
}

void KeyMapper::handleButtonObjectSettings(InputButtonObject* button, std::string settingsString)
{
	std::string setting;
	char settingChar;
	int settingValue;
	int index;

	while(settingsString != "")
	{
		index = settingsString.find_first_of(" ");
		if(index < 0)
			break;

		setting = settingsString.substr(0, index);
		settingsString = settingsString.substr(index + 1);

		if(setting.size() < 3)
			continue;

		settingChar = setting[0];
		index = setting.find_first_of("=");
		if(index == setting.npos)
			continue;

		setting = setting.substr(index + 1);
		settingValue = Converter::StrToInt(setting);
		if(settingValue < 0)
			continue;

		switch(settingChar)
		{
		case 'F': //Float key mapping
			button->addFloatMapping(settingValue);
			break;
		case 'B': //Bool key mapping
			button->addBoolMapping(settingValue);
			break;
		case 'I': //Inverted
			button->setInverted(settingValue > 0);
			break;
		default:
			break;
		};
	}
}

void KeyMapper::handleHatSwitchObjectSettings(InputHatSwitchObject* hatSwitch, std::string settingsString)
{
	std::string setting;
	char settingChar;
	int settingValue;
	int index;

	while(settingsString != "")
	{
		index = settingsString.find_first_of(" ");
		if(index < 0)
			break;

		setting = settingsString.substr(0, index);
		settingsString = settingsString.substr(index + 1);

		if(setting.size() < 3)
			continue;

		settingChar = setting[0];
		index = setting.find_first_of("=");
		if(index = setting.npos)
			continue;

		setting = setting.substr(index);
		settingValue = Converter::StrToInt(setting);
		if(settingValue < 0)
			continue;

		switch(settingChar)
		{
		//Figure shit out
		default:
			break;
		};
	}
}

void KeyMapper::handleTriggerObjectSettings(InputTriggerObject* trigger, std::string settingsString)
{
	std::string setting;
	char settingChar;
	int settingValue;
	float settingFloatValue;
	int index;

	while(settingsString != "")
	{
		index = settingsString.find_first_of(" ");
		if(index < 0)
			break;

		setting = settingsString.substr(0, index);
		settingsString = settingsString.substr(index);

		if(setting.size() < 3)
		{
			settingsString = settingsString.substr(setting.size() + 1);
			continue;
		}

		settingChar = setting[0];
		index = setting.find_first_of("=");
		if(index == setting.npos)
			continue;

		setting = setting.substr(index);
		settingValue = Converter::StrToInt(setting);
		if(settingValue < 0)
			continue;

		switch(settingChar)
		{
		case 'F': //Float key mapping
			trigger->addFloatMapping(settingValue);
			break;
		case 'B': //Bool key mapping
			trigger->addBoolMapping(settingValue);
			break;
		case 'D': //Deadzone
			settingFloatValue = Converter::StrToFloat(setting);
			trigger->setDeadZone(settingFloatValue);
			break;
		case 'T': //Trigger value
			settingFloatValue = Converter::StrToFloat(setting);
			trigger->setTriggerValue(settingFloatValue);
			break;
		case 'I': //Inverted
			trigger->setInverted(settingValue > 0);
			break;
		case 'S': //Sensitivity
			//Set sensitivity
			break;
		default:
			break;
		};
	}
}

std::string KeyMapper::getGUIDstring(GUID* guid)
{
	std::string guidString = "guid: ";

	guidString += Converter::UIntToStr((unsigned int)guid->Data1) + "-";
	guidString += Converter::UIntToStr((unsigned int)guid->Data2) + "-";
	guidString += Converter::UIntToStr((unsigned int)guid->Data3) + "-";
	
	for(int i = 0; i < 8; i++)
		guidString += guid->Data4[i];

	return guidString;
}

std::string KeyMapper::getAxesString(std::vector<InputAxisObject*>* axes)
{
	std::string axesString = "";
	InputAxisObject* axis = nullptr;

	for(unsigned int i = 0; i < axes->size(); i++)
	{
		axesString += "#A" + Converter::IntToStr(i) + ": ";
		axis = axes->at(i);
		
		axesString += "D=" + Converter::FloatToStr(axis->getDeadZone()) + " ";
		axesString += "I=" + Converter::IntToStr((int)axis->isInverted()) + " ";

		std::vector<int> mappings = axis->getFloatMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
			axesString += "F=" + Converter::IntToStr(mappings[i]) + " ";

		mappings = axis->getBoolMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
			axesString += "B=" + Converter::IntToStr(mappings[i]) + " ";
	}

	return axesString;
}

std::string KeyMapper::getButtonString(std::vector<InputButtonObject*>* buttons)
{
	std::string buttonString = "";
	InputButtonObject* button = nullptr;

	for(unsigned int i = 0; i < buttons->size(); i++)
	{
		button = buttons->at(i);

		buttonString += "#B" + Converter::IntToStr(button->getKey()) + ": ";
		
		buttonString += "I=" + Converter::IntToStr((int)button->isInverted()) + " ";

		std::vector<int> mappings = button->getFloatMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
		{
			buttonString += "F=" + Converter::IntToStr(mappings[i]) + " ";
		}

		mappings = button->getBoolMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
		{
			buttonString += "B=" + Converter::IntToStr(mappings[i]) + " ";
		}
	}

	return buttonString;
}

std::string KeyMapper::getHatSwitchString(std::vector<InputHatSwitchObject*>* hatSwitches)
{
	return "";
}

std::string KeyMapper::getTriggerString(std::vector<InputTriggerObject*>* triggers)
{
	std::string triggerString = "";
	InputTriggerObject* trigger = nullptr;

	for(unsigned int i = 0; i < triggers->size(); i++)
	{
		triggerString += "#T" + Converter::IntToStr(i) + ": ";
		trigger = triggers->at(i);
		
		triggerString += "I=" + Converter::IntToStr((int)trigger->isInverted()) + " ";
		triggerString += "T=" + Converter::FloatToStr(trigger->getTriggerValue()) + " ";
		triggerString += "D=" + Converter::FloatToStr(trigger->getDeadZone()) + " ";

		std::vector<int> mappings = trigger->getFloatMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
			triggerString += "F=" + Converter::IntToStr(mappings[i]) + " ";

		mappings = trigger->getBoolMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
			triggerString += "B=" + Converter::IntToStr(mappings[i]) + " ";
	}

	return triggerString;
}

std::string KeyMapper::removeComment(std::string str)
{
	int strIndex = str.find_first_of("//");
	return str.substr(0, strIndex);
}

std::string KeyMapper::configMessage()
{
	std::string message = "";

	message += "// This file contains key mappings.\n";

	return message;
}