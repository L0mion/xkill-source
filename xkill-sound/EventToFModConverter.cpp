#include "EventToFModConverter.h"

#include <sstream>

EventToFModConverter::EventToFModConverter()
{

}

EventToFModConverter::~EventToFModConverter()
{

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
	int strIndex;
	int eventIndex, fmodIndex;

	strIndex = conversionRow.find_first_not_of(' ');	//Remove whitespace
	if(strIndex == conversionRow.npos) return;

	conversionRow = conversionRow.substr(strIndex);

	strIndex = conversionRow.find_first_of("//");
	if(strIndex == 0) return; //It's a comment

	strIndex = conversionRow.find_first_of('=');
	if(strIndex == conversionRow.npos) return; //Not a correctly formatted row

	fmodIndex = stringToInt(conversionRow.substr(0, strIndex - 1));
	if(fmodIndex < 0) return; //If true then it wasn't a number

	conversionRow = conversionRow.substr(strIndex + 1);
	strIndex = conversionRow.find_first_not_of(' ');
	conversionRow = conversionRow.substr(strIndex);
	strIndex = conversionRow.find_first_of("//");
	conversionRow = conversionRow.substr(0, strIndex);
	strIndex = conversionRow.size() - 1;
	for(; conversionRow[strIndex] == ' '; strIndex--);

	eventIndex = stringToInt(conversionRow);
	if(eventIndex < 0)	//If true then it wasn't a number
		return;			//Should later check if it was a eventname

	eventToFModArray_.push_back(std::pair<int, int>(eventIndex, fmodIndex));
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
