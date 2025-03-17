#pragma once
#include <pch.hpp>

// *Deal with Windows UTF-16
namespace sglf
{
	wchar_t* toWideString(const char *string);
}
