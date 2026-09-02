#pragma once
#include <Windows.h>
#include <string>

class StringUtils
{
public:
	static std::string wcharToString(const WCHAR* wstr);
	static std::string wcharToStringWithLen(const WCHAR* wstr, int sizeNeeded);
};