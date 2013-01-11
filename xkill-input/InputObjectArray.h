#pragma once

#include "InputObject.h"
#include <vector>

class InputObjectArray
{
public:
	InputObjectArray(){}
	~InputObjectArray(){}

	std::vector<InputObject*> inputObjects;
};
