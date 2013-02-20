#include "EventToFModConverter.h"

#include <xkill-utilities/EventType.h>
#include <xkill-utilities/Converter.h>
#include "FileParser.h"
#include <sstream>

EventToFModConverter::EventToFModConverter(std::vector<std::string> fmodEventNames)
{
	fmodEventNames_ = fmodEventNames;
}

EventToFModConverter::~EventToFModConverter()
{

}

bool EventToFModConverter::init(std::string filepath)
{
	fillNameConversionArray(filepath);

	return true;
}

void EventToFModConverter::fillNameConversionArray(std::string filepath)
{
	std::string message;

	message = "#" + Converter::ULongToStr(Converter::HashString(configMessage())) + "\n";
	message += configMessage();

	FileParser fp(message);
	fp.setFilePath(filepath);
	fp.setFileName("events.cfg");

	fp.startReading();

	std::string row = "";

	if(!fp.isEmpty())
	{
		row = fp.getNextRow();
	}

	if(!isValid(row))
	{
		fp.clean();
		fp.deleteFile();

		fp.startReading();
	}

	int number;
	std::string numberStr, name;

	while(!fp.isEmpty())
	{
		row = fp.getNextRow();
		if(splitRowIntoValues(row, numberStr, name) && (number = stringToInt(numberStr)) >= 0)
		{
			eventNameToNumberEvent_.push_back(std::pair<std::string, int>(name, number));
		}
	}
}

int EventToFModConverter::getFModIndex(int eventIndex)
{
	int result = -1;

	for(unsigned int i = 0; i < eventToFModArray_.size(); i++)
	{
		if(eventToFModArray_[i].first == eventIndex)
		{
			result = eventToFModArray_[i].second;
		}
	}

	return result;
}

void EventToFModConverter::addConversion(std::string conversionRow)	
{
	std::string name;
	std::string numberStr;
	int number;

	if(splitRowIntoValues(conversionRow, numberStr, name))
	{
		if((number = stringToInt(numberStr)) < 0)	// If true: numberstr wasn't a number, check if it's a fmod event name
		{
			for(unsigned int i = 0; i < fmodEventNames_.size(); i++)
			{
				if(fmodEventNames_[i] == numberStr)
				{
					number = i;
				}
			}
		}

		if(number < -1)
			return;

		std::vector<int> eventIndices;
		int nameIndex = stringToInt(name);
		if(nameIndex < 0)	//If true then it wasn't a number
		{
			bool matchFound = false;

			for(unsigned int i = 0; i < eventNameToNumberEvent_.size(); i++)
			{
				if(eventNameToNumberEvent_[i].first == name)	//Search for matching event name
				{
					eventIndices.push_back(eventNameToNumberEvent_[i].second);
					matchFound = true;
				}
			}

			if(!matchFound)
				return;
		}
		else
		{
			eventIndices.push_back(nameIndex);
		}

		for(unsigned int i = 0; i < eventIndices.size(); i++)
		{
			eventToFModArray_.push_back(std::pair<int, int>(eventIndices[i], number));
		}
	}
}

bool EventToFModConverter::splitRowIntoValues(std::string row, std::string& firstStr, std::string& secondStr)
{
	int strIndex;

	strIndex = row.find_first_of("//");		//Remove everything after comment
	row = row.substr(0, strIndex);

	strIndex = row.find_first_of('=');
	if(strIndex == row.npos) return false;	//Not a correctly formatted row

	firstStr = row.substr(0, strIndex - 1);
	firstStr = removeWhiteSpaceAtBeginningAndEnd(firstStr);

	row = row.substr(strIndex + 1);			//Get string after '='

	secondStr = removeWhiteSpaceAtBeginningAndEnd(row);
	return true;
}

std::string EventToFModConverter::removeWhiteSpaceAtBeginningAndEnd(std::string str)
{
	int strIndex;

	strIndex = str.find_first_not_of(' ');	//Remove whitespace at the beginning of the string
	if(strIndex == str.npos)
		return "";

	str = str.substr(strIndex);

	strIndex = str.size();
	for(; (strIndex >= 0) && (str[strIndex] == ' '); strIndex--);
	if(strIndex < 0)
		return "";

	str = str.substr(0, strIndex);		//Remove whitespace at the end of the string

	return str;
}

bool EventToFModConverter::isValid(std::string row)
{
	int strIndex;

	strIndex = row.find_first_of("//");		//Remove everything after comment
	row = row.substr(0, strIndex);

	removeWhiteSpaceAtBeginningAndEnd(row);

	if(row == "" || row[0] != '#')
		return false;

	std::string hashMessage = configMessage();
	hashMessage += __TIME__;
	hashMessage += __DATE__;

	std::string hash = "#" + Converter::ULongToStr(Converter::HashString(hashMessage));

	return hash == row;
}

int EventToFModConverter::stringToInt(std::string str)
{
	std::stringstream ss(str);
	int n;
	ss >> n;

	if(ss.fail())
	{
		return -1;
	}

	return n;
}

std::string EventToFModConverter::intToString(int n)
{
	std::string str;
	std::stringstream ss;
	ss << n;
	str = ss.str();

	if(ss.fail())
	{
		return "";
	}

	return str;
}

std::string EventToFModConverter::configMessage()
{
	int offset = Event_PlaySound::SOUND_LAST;
	std::string message;

	message += "// This file is a conversion table between in-game event numbers and names.\n";
	message += "// Use following formatting:\n";
	message += "// <event number> = <event name>\n";
	message += "// The names are allowed to contain spaces.\n";
	message += "// The names are not allowed to begin with a number as this will be treated as\n";
	message += "// a event number. \n";
	message += "// This file will be used to convert fmod events to in-game events.\n\n";

	std::vector<std::pair<int, std::string>> table;		

	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_DEATH,			"Player Death"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_FIRE,			"Fire Gun"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_GAME_MUSIC,		"Game Music"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_HIT,				"Player Hit"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_MENU_MUSIC,		"Menu Music"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_RESPAWN,			"Player Respawn"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_WALK,			"Player Walk"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_BUTTON_CLICK,	"Button Click"));
	table.push_back(std::pair<int, std::string>(Event_PlaySound::SOUND_OPENING_ANIMATION,"Opening Animation"));
	//table.push_back(std::pair<int, std::string>(EVENT_CREATE_PROJECTILE + offset,		"Fire Gun"));
	//table.push_back(std::pair<int, std::string>(EVENT_PLAYERDEATH + offset,			"Player Death"));
	//table.push_back(std::pair<int, std::string>(EVENT_CREATE_PICKUPABLE + offset,		"Spawn Pickupable"));
	//table.push_back(std::pair<int, std::string>(EVENT_CREATE_EXPLOSIONSPHERE + offset,"Explosion"));

	sortConfigMessageTable(table);

	for(unsigned int i = 0; i < table.size(); i++)
	{
		message += Converter::IntToStr(table[i].first) + " = " + table[i].second + "\n";
	}

	return message;
}

void EventToFModConverter::sortConfigMessageTable(std::vector<std::pair<int, std::string>>& table)
{	//Use bubblesort as the table will be short
	for(unsigned int i = 0; i < table.size(); i++)
	{
		for(unsigned int j = i; j < table.size(); j++)
		{
			if(table[j] < table[i])
			{
				swap(table, i, j);
			}
		}
	}
}

void EventToFModConverter::swap(std::vector<std::pair<int, std::string>>& table, int first, int second)
{
	std::pair<int, std::string> temp = table[first];
	table[first] = table[second];
	table[second] = temp;
}
