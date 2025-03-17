#include <util.hpp>

wchar_t* sglf::toWideString(const char *string)
{
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, string, -1, nullptr, 0);
	wchar_t *wideString = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, string, -1, wideString, bufferSize);
	return wideString;
}