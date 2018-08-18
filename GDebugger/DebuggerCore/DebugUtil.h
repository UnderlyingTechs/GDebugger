#pragma once
#include <windows.h>
#include <string>

/**
 * ¿í×Ö·û´®×ª±ê×¼×Ö·û´®
 */
void WCHARToString(std::string& dest, WCHAR* wstr);

/**
 * ×Ö·û´®×ª±ê×¼¿í×Ö·û´®
 */
void StringToWstring(std::wstring& dest, std::string str);
