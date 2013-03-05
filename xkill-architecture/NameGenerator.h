#pragma once

#include <vector>

class NameGenerator
{
private:
	std::vector<std::string> names;

public:
	NameGenerator(){};
	~NameGenerator(){};
	void reset();
	void addName(std::string name);
	std::string getName();
};