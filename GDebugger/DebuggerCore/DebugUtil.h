#pragma once
#include <windows.h>
#include <string>

/**
 * ¿í×Ö·û´®×ª±ê×¼×Ö·û´®
 */
_declspec(dllexport) void WCHARToString(std::string& dest, WCHAR* wstr);

/**
 * ×Ö·û´®×ª±ê×¼¿í×Ö·û´®
 */
_declspec(dllexport) void StringToWstring(std::wstring& dest, std::string str);
