#pragma once

#include <vector>
#include <xkill-utilities/Math.h>

class NameGenerator
{
private:
	std::vector<Float3> colors;
	std::vector<std::string> names;

public:
	NameGenerator(){};
	~NameGenerator(){};
	void reset();
	void resetNames();
	void resetColors();
	void addName(std::string name);
	std::string getName();
	void addColor(Float3 color);
	Float3 getColor();
};