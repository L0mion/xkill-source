#pragma once

#include <vector>
#include <string>

class EventToFModConverter
{
public:
	EventToFModConverter(std::vector<std::string> fmodEventNames);
	~EventToFModConverter();

	bool init(std::string filepath);

	int getFModIndex(int eventIndex);

	//! Takes a formatted string and makes an conversion between events and fmod
	/*!
	Takes a string formatted as follows:

	// Comment
	[fmodindex] = [eventindex]
	[fmodindex] = [eventname]
	[fmodname] = [eventindex]
	[fmodname] = [eventname]

	*/
	void addConversion(std::string conversionRow);

private:
	std::vector<std::pair<int, int>> eventToFModArray_;
	std::vector<std::pair<std::string, int>> eventNameToNumberEvent_;
	std::vector<std::string> fmodEventNames_;

	void fillNameConversionArray(std::string filepath);

	bool splitRowIntoValues(std::string row, std::string& firstStr, std::string& secondStr);
	std::string removeWhiteSpaceAtBeginningAndEnd(std::string str);

	bool isValid(std::string row);
	int stringToInt(std::string str);
	std::string intToString(int n);
	std::string configMessage();

	void sortConfigMessageTable(std::vector<std::pair<int, std::string>>& table);
	void swap(std::vector<std::pair<int, std::string>>& table, int first, int second);
};