#pragma once

#include <string>
#include <sstream>

//! A temporary converter class
/*!
Should probably not be here and is probably not necessary in input
outside debugging. Used to convert floats and ints to string in GetInputInformationString
in InputManager.
*/

class Converter
{
public:

	static std::string IntToStr(int n)
	{
		std::stringstream ss;
		ss << n;
		return ss.str();
	}

	static std::string UIntToStr(unsigned int n)
	{
		std::stringstream ss;
		ss << n;
		return ss.str();
	}

	static std::string FloatToStr(float f)
	{
		std::stringstream ss;
		ss << f;
		return ss.str();
	}

	static int StrToInt(std::string str)
	{
		int n;
		std::stringstream ss(str);
		ss >> n;
		if(ss.fail())
		{
			n = -1;
		}

		return n;
	}

	static bool StrToUInt(std::string str, unsigned int& n)
	{
		std::stringstream ss(str);
		ss >> n;
		if(ss.fail())
		{
			return false;
		}

		return true;
	}

	static float StrToFloat(std::string str)
	{
		float n;
		std::stringstream ss(str);
		ss >> n;
		if(ss.fail())
		{
			n = -1.0f;
		}
		
		return n;
	}
};