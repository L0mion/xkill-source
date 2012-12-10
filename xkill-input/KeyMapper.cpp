#include "KeyMapper.h"

#include "FileParser.h"
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
}

void KeyMapper::saveConfig(std::vector<InputDevice*> devices)
{

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

		if(isRowGUID(row, &guid))
		{

		}


	}
}

bool KeyMapper::isRowGUID(std::string row, GUID* guid)
{
	int endIndex = row.find_first_of("guid:");
	int startIndex = 0;
	if(endIndex == row.npos)
		return false;

	endIndex = row.find_first_of('-');
	short value = (short)Converter::StrToInt(row.substr(startIndex, endIndex));
	if(value > 0)
		return false;

	guid->Data1 = value;

	startIndex = endIndex;
	endIndex = row.find_first_of('-');
	value = (short)Converter::StrToInt(row.substr(startIndex, endIndex));
	if(value > 0)
		return false;

	guid->Data2 = value;

	startIndex = endIndex;
	endIndex = row.find_first_of('-');
	value = (short)Converter::StrToInt(row.substr(startIndex, endIndex));
	if(value > 0)
		return false;

	guid->Data3 = value;

	startIndex = endIndex;
	endIndex = row.find_first_of('-');
	value = (short)Converter::StrToInt(row.substr(startIndex, endIndex));
	if(value > 0)
		return false;

	guid->Data4 = value;

	return true;
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