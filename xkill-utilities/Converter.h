#pragma once

#include <string>
#include <sstream>
#include <iomanip>

//! A converter class

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

	static std::string ULongToStr(unsigned long n)
	{
		std::stringstream ss;
		ss << n;
		return ss.str();
	}

	static std::string FloatToStr(float f)
	{
		std::stringstream ss;
		ss << std::fixed;
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

	static bool StrToULong(std::string str, unsigned long& n)
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

/*  //////////////////////////////////////////////////////////////////
Hash algorithm djb2 from: http://www.cse.yorku.ca/~oz/hash.html		//
*/  //////////////////////////////////////////////////////////////////

	static unsigned long HashString(std::string str)
	{
		unsigned long hash = 5381;

		for(unsigned int i = 0; i < str.size(); i++)
		{
			hash = ((hash << 5) + hash) + str[i];
		}

		return hash;
	}
};