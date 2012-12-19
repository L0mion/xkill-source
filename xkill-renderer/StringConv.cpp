#include <Windows.h>

#include "StringConv.h"

std::string uintToString(const unsigned int uint)
{
	return std::to_string(uint);
}
wchar_t* stringToWstr(std::string string)
{
	int wchars_num = MultiByteToWideChar(
		CP_UTF8 , 
		0 , 
		string.c_str(), 
		-1, 
		NULL , 
		0);
	wchar_t* wstr = new wchar_t[wchars_num];

	MultiByteToWideChar(
		CP_UTF8, 
		0,
		string.c_str(),
		-1, 
		wstr , 
		wchars_num);

	return wstr;
}