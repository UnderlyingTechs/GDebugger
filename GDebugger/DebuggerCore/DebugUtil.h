#pragma once
#include <windows.h>
#include <string>

/**
 * ���ַ���ת��׼�ַ���
 */
_declspec(dllexport) void WCHARToString(std::string& dest, WCHAR* wstr);

/**
 * �ַ���ת��׼���ַ���
 */
_declspec(dllexport) void StringToWstring(std::wstring& dest, std::string str);
