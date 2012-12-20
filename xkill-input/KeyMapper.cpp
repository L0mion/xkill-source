#include "KeyMapper.h"

#include "FileParser.h"
#include "InputDevice.h"
#include "Converter.h"

KeyMapper::KeyMapper()
{
	
}

KeyMapper::~KeyMapper()
{
	writeFileList();

	delete fileParser_;
}

bool KeyMapper::init(std::string configFilePath)
{
	fileParser_ = new FileParser("");
	fileParser_->setFilePath(configFilePath + "Keymappings/");

	parseFileList();

	return true;
}

void KeyMapper::saveConfig(std::vector<InputDevice*> devices)
{
	bool existed;

	for(unsigned int i = 0; i < devices.size(); i++)
	{
		existed = false;

		for(unsigned int j = 0; j < fileList_.size(); j++)
		{
			if(devices[i]->GetGUID() == fileList_[j].first)
			{
				fileParser_->setFileName(fileList_[j].second);

				existed = true;
				break;
			}
		}

		if(!existed)
		{
			GUID guid = devices[i]->GetGUID();
			std::string fileName = Converter::IntToStr(guid.Data1);
			fileName += Converter::IntToStr(guid.Data2);
			fileName += Converter::IntToStr(guid.Data3);
			fileName += ".kmp";
			fileParser_->setFileName(fileName);

			fileList_.push_back(std::pair<GUID, std::string>(guid, fileName));
		}

		writeToFile(devices[i]);
	}
}

void KeyMapper::loadConfig(std::vector<InputDevice*> devices)
{
	for(unsigned int i = 0; i < devices.size(); i++)
	{
		getConfigForNewController(devices[i]);
	}
}

void KeyMapper::getConfigForNewController(InputDevice* device)
{
	bool mappingsFound = false;

	for(unsigned int j = 0; j < fileList_.size(); j++)
	{
		if(device->GetGUID() == fileList_[j].first)
		{
			fileParser_->setFileName(fileList_[j].second);
			if(parseFile(device))
				mappingsFound = true;

			break;
		}
	}

	if(!mappingsFound)
	{
		device->setStandardMappings();
	}

	device->createObjectVectors();
}

void KeyMapper::parseFileList()
{
	std::string row;
	GUID guid;
	std::string fileName;

	fileParser_->setFileName("guid_list.gli");
	fileParser_->startReading();

	while(!fileParser_->isEmpty())
	{
		row = fileParser_->getNextRow();

		if(isGUIDRow(row, guid, fileName))
			fileList_.push_back(std::pair<GUID, std::string>(guid, fileName));
	}
}

void KeyMapper::writeFileList()
{
	fileParser_->setFileName("guid_list.gli");
	std::string row;

	fileParser_->startWriting();
	
	for(unsigned int i = 0; i < fileList_.size(); i++)
	{
		row = getGUIDstring(&fileList_[i].first);
		row += " = #" + fileList_[i].second + "#";

		fileParser_->writeRow(row);
	}

	fileParser_->doneWriting();
}

bool KeyMapper::isGUIDRow(std::string row, GUID& guid, std::string& fileName)
{
	int endIndex = row.find_first_of("guid");
	int startIndex = 0;
	std::string guidChars;

	if(endIndex == row.npos)
		return false;

	startIndex = endIndex + 5;
	row = row.substr(startIndex);
	endIndex = row.find_first_of('-');
	unsigned int value = 0;
	if(!Converter::StrToUInt(row.substr(0, endIndex), value))
		return false;

	guid.Data1 = value;

	startIndex = endIndex + 1;
	row = row.substr(startIndex);
	endIndex = row.find_first_of('-');
	if(!Converter::StrToUInt(row.substr(0, endIndex), value))
		return false;

	guid.Data2 = value;

	startIndex = endIndex + 1;
	row = row.substr(startIndex);
	endIndex = row.find_first_of('-');
	if(!Converter::StrToUInt(row.substr(0, endIndex), value))
		return false;

	guid.Data3 = value;

	startIndex = endIndex + 1;
	row = row.substr(startIndex);

	endIndex = row.find_first_of("=");
	guidChars = row.substr(0, endIndex);

	for(unsigned int i = 0; i < guidChars.size() && i < 8; i++)
		guid.Data4[i] = guidChars[i];

	row = row.substr(endIndex + 1);

	startIndex = row.find_first_of("#");
	if(startIndex == row.npos)
		return false;

	endIndex = row.find_first_of("#", startIndex + 1);

	fileName = row.substr(startIndex + 1, endIndex - 2);

	if(fileName == "")
		return false;

	return true;
}

bool KeyMapper::parseFile(InputDevice* device)
{
	std::string row;

	if(!fileParser_->startReading())
		return false;

	if(fileParser_->isEmpty())
		return false;

	while(!fileParser_->isEmpty())
	{
		row = fileParser_->getNextRow();
		row = removeComment(row);

		extractSettingsFromRow(row, device);
	}

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

void KeyMapper::writeToFile(InputDevice* device)
{
	fileParser_->startWriting();

	std::string row = "";

	row = getAxesString(&device->axes_);
	if(row != "")
		fileParser_->writeRow(row);

	row = getButtonString(&device->buttons_);
	if(row != "")
		fileParser_->writeRow(row);

	row = getTriggerString(&device->triggers_);
	if(row != "")
		fileParser_->writeRow(row);

	fileParser_->doneWriting();
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
		if(button != nullptr)
			handleButtonObjectSettings(button, settingsString);
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
	std::vector<std::pair<char, std::string>>* settings = settingsExtracter(settingsString);
	std::pair<char, std::string>* settingsPair;

	unsigned int intValue;
	float floatValue;

	for(unsigned int i = 0; i < settings->size(); i++)
	{
		settingsPair = &settings->at(i);
		switch(settingsPair->first)
		{
		case 'F': //Float key mapping
			if(Converter::StrToUInt(settingsPair->second, intValue))
				axis->addFloatMapping(intValue);
			break;
		case 'B': //Bool key mapping
			if(Converter::StrToUInt(settingsPair->second, intValue))
				axis->addBoolMapping(intValue);
			break;
		case 'D': //Deadzone
			floatValue = Converter::StrToFloat(settingsPair->second);
			if(floatValue >= 0.0f)
				axis->setDeadZone(floatValue);
			break;
		case 'T': //Trigger value
			//Set trigger value
			break;
		case 'I': //Inverted
			if(Converter::StrToUInt(settingsPair->second, intValue))
				axis->setInverted(intValue > 0);
			break;
		case 'S': //Sensitivity
			floatValue = Converter::StrToFloat(settingsPair->second);
			if(floatValue >= 0.0f)
				axis->setSensitivity(floatValue);
			break;
		default:
			break;
		};
	}

	delete settings;
}

void KeyMapper::handleButtonObjectSettings(InputButtonObject* button, std::string settingsString)
{
	std::vector<std::pair<char, std::string>>* settings = settingsExtracter(settingsString);
	std::pair<char, std::string>* settingsPair;

	unsigned int intValue;
	float floatValue;

	for(unsigned int i = 0; i < settings->size(); i++)
	{
		settingsPair = &settings->at(i);
		switch(settingsPair->first)
		{
		case 'F': //Float key mapping
			if(Converter::StrToUInt(settingsPair->second, intValue))
				button->addFloatMapping(intValue);
			break;
		case 'B': //Bool key mapping
			if(Converter::StrToUInt(settingsPair->second, intValue))
				button->addBoolMapping(intValue);
			break;
		case 'I': //Inverted
			if(Converter::StrToUInt(settingsPair->second, intValue))
				button->setInverted(intValue > 0);
			break;
		case 'S': //Sensitivity
			floatValue = Converter::StrToFloat(settingsPair->second);
			if(floatValue >= 0.0f)
				button->setSensitivity(floatValue);
			break;
		default:
			break;
		};
	}

	delete settings;
}

void KeyMapper::handleTriggerObjectSettings(InputTriggerObject* trigger, std::string settingsString)
{
	std::vector<std::pair<char, std::string>>* settings = settingsExtracter(settingsString);
	std::pair<char, std::string>* settingsPair;

	unsigned int intValue;
	float floatValue;

	for(unsigned int i = 0; i < settings->size(); i++)
	{
		settingsPair = &settings->at(i);
		switch(settingsPair->first)
		{
		case 'F': //Float key mapping
			if(Converter::StrToUInt(settingsPair->second, intValue))
				trigger->addFloatMapping(intValue);
			break;
		case 'B': //Bool key mapping
			if(Converter::StrToUInt(settingsPair->second, intValue))
				trigger->addBoolMapping(intValue);
			break;
		case 'D': //Deadzone
			floatValue = Converter::StrToFloat(settingsPair->second);
			if(floatValue >= 0.0f)
				trigger->setDeadZone(floatValue);
			break;
		case 'T': //Trigger value
			floatValue = Converter::StrToFloat(settingsPair->second);
			if(floatValue >= 0.0f)
				trigger->setTriggerValue(floatValue);
			break;
		case 'I': //Inverted
			if(Converter::StrToUInt(settingsPair->second, intValue))
				trigger->setInverted(intValue > 0);
			break;
		case 'S': //Sensitivity
			floatValue = Converter::StrToFloat(settingsPair->second);
			if(floatValue >= 0.0f)
				trigger->setSensitivity(floatValue);
			break;
		default:
			break;
		};
	}

	delete settings;
}

std::vector<std::pair<char, std::string>>* KeyMapper::settingsExtracter(std::string settingsString)
{
	std::string setting;
	char settingChar;
	int index;
	std::vector<std::pair<char, std::string>>* settings = new std::vector<std::pair<char, std::string>>();

	while(settingsString != "")
	{
		index = settingsString.find_first_of(" ");

		if(index < 0)
		{
			if(settingsString.size() <= 0)
			{
				break;
			}
			else
			{
				index = settingsString.size();
			}
		}
		else if(index == 0)
		{
			index = 1;
		}

		setting = settingsString.substr(0, index);
		settingsString = settingsString.substr(index);

		if(setting.size() < 3)
			continue;

		settingChar = setting[0];
		index = setting.find_first_of('=');
		if(index == setting.npos)
			continue;

		setting = setting.substr(index + 1);

		settings->push_back(std::pair<char, std::string>(settingChar, setting));
	}

	return settings;
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
		axesString += "S=" + Converter::FloatToStr(axis->getSensitivity()) + " ";

		std::vector<int> mappings = axis->getFloatMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
			axesString += "F=" + Converter::IntToStr(mappings[j]) + " ";

		mappings = axis->getBoolMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
			axesString += "B=" + Converter::IntToStr(mappings[j]) + " ";
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
		buttonString += "S=" + Converter::FloatToStr(button->getSensitivity()) + " ";

		std::vector<int> mappings = button->getFloatMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
		{
			buttonString += "F=" + Converter::IntToStr(mappings[j]) + " ";
		}

		mappings = button->getBoolMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
		{
			buttonString += "B=" + Converter::IntToStr(mappings[j]) + " ";
		}
	}

	return buttonString;
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
		triggerString += "S=" + Converter::FloatToStr(trigger->getSensitivity()) + " ";

		std::vector<int> mappings = trigger->getFloatMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
		{
			triggerString += "F=" + Converter::IntToStr(mappings[j]) + " ";
		}

		mappings = trigger->getBoolMappings();

		for(unsigned int j = 0; j < mappings.size(); j++)
		{
			triggerString += "B=" + Converter::IntToStr(mappings[j]) + " ";
		}
	}

	return triggerString;
}

std::string KeyMapper::removeComment(std::string str)
{
	int strIndex = str.find_first_of("//");
	return str.substr(0, strIndex);
}