#pragma once

#include <vector>
#include <string>

class EventToFModConverter
{
public:
	EventToFModConverter();
	~EventToFModConverter();

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

	int stringToInt(std::string str);
};