#pragma once
#include <windows.h>
#include <string>

/**
 * ���ַ���ת��׼�ַ���
 */
void WCHARToString(std::string& dest, WCHAR* wstr);

/**
 * �ַ���ת��׼���ַ���
 */
void StringToWstring(std::wstring& dest, std::string str);
