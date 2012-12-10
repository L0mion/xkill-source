#pragma once

#include <string>

#include "InputDevice.h"

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

	bool isRowGUID(std::string row, GUID* guid);


	std::string removeComment(std::string str);
	std::string configMessage();
};