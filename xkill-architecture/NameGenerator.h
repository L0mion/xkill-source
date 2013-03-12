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
	void resetNames();
	void resetColors()
	{
		//
		// Add colors
		//

		addColor(Float3(0.0f, 1.0f, 0.0));
		addColor(Float3(1.0f, 0.0f, 0.0));
		addColor(Float3(0.0f, 1.0f, 1.0));
		addColor(Float3(1.0f, 0.4f, 0.0));
		addColor(Float3(0.7f, 0.0f, 1.0));
	}
	void addName(std::string name);
	std::string getName();
	void addColor(Float3 color)
	{
		colors.push_back(color);
	}
	Float3 getColor()
	{
		// Reset colors if all have been picked
		if(colors.size() <= 0)
			resetColors();

		// Pick random color
		int numColors = colors.size();
		int index = Math::randomInt(0, numColors-1);
		Float3 color = colors.at(index);

		// Remove name using Swap-Trick
		colors.at(index) = colors.back();
		colors.pop_back();


		// Return name
		return color;
	}
};