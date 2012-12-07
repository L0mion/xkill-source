#pragma once

#include <vector>
#include <string>

class EventToFModConverter
{
public:
	EventToFModConverter();
	~EventToFModConverter();

	bool init(std::string filepath);

	int getFModIndex(int eventIndex);

	//! Takes a formatted string and makes an conversion between events and fmod
	/*!
	Takes a string formatted as follows:

	// Comment
	[fmodindex] = [eventindex]
	[fmodindex] = [eventname]

	*/
	void addConversion(std::string conversionRow);

private:
	std::vector<std::pair<int, int>> eventToFModArray_;
	std::vector<std::pair<std::string, int>> eventNameToNumberEvent_;

	void fillNameConversionArray(std::string filepath);

	bool splitRowIntoValues(std::string row, int& number, std::string& name);
	std::string removeWhiteSpaceAtBeginningAndEnd(std::string str);

	int stringToInt(std::string str);
	std::string configMessage();
};