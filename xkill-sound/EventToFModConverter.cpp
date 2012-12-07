#include "EventToFModConverter.h"

#include "FileParser.h"
#include <sstream>

EventToFModConverter::EventToFModConverter()
{

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
	FileParser fp(configMessage());
	fp.setFilePath(filepath);
	fp.setFileName("events.cfg");
	fp.startReading();

	int number;
	std::string name, row;

	while(!fp.isEmpty())
	{
		row = fp.getNextRow();
		if(splitRowIntoValues(row, number, name))
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
	int number;

	if(splitRowIntoValues(conversionRow, number, name))
	{
		int eventIndex = stringToInt(name);
		if(eventIndex < 0)	//If true then it wasn't a number
		{
			bool matchFound = false;

			for(unsigned int i = 0; i < eventNameToNumberEvent_.size(); i++)
			{
				if(eventNameToNumberEvent_[i].first == name)	//Search for matching event name
				{
					eventIndex = eventNameToNumberEvent_[i].second;
					matchFound = true;
					break;
				}
			}

			if(!matchFound)
				return;		//Should later check if it was a eventname
		}

		eventToFModArray_.push_back(std::pair<int, int>(eventIndex, number));
	}
}

bool EventToFModConverter::splitRowIntoValues(std::string row, int& number, std::string& name)
{
	int strIndex;

	strIndex = row.find_first_of("//");		//Remove everything after comment
	row = row.substr(0, strIndex);

	strIndex = row.find_first_of('=');
	if(strIndex == row.npos) return false;	//Not a correctly formatted row

	number = stringToInt(row.substr(0, strIndex - 1));
	if(number < 0) return false;			//If true then it wasn't a number

	row = row.substr(strIndex + 1);			//Get string after '='

	name = removeWhiteSpaceAtBeginningAndEnd(row);
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

std::string EventToFModConverter::configMessage()
{
	std::string message;

	message += "// This file is a conversion table between in-game event numbers and names.\n";
	message += "// Use following formatting:\n";
	message += "// <event number> = <event name>\n";
	message += "// The names are allowed to contain spaces.\n";
	message += "// The names are not allowed to begin with a number as this will be treated as\n";
	message += "// a event number. \n";
	message += "// This file will be used to convert fmod events to in-game events.\n";

	return message;
}
