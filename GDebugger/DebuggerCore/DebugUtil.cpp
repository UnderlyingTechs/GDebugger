#include "DebugUtil.h"

void WCHARToString(std::string& dest, WCHAR* wstr)
{
	WCHAR* pText = wstr;
	const DWORD length = WideCharToMultiByte(
		CP_ACP,
		NULL,
		pText,
		-1,
		nullptr,
		0,
		nullptr,
		nullptr);

	const auto tempText = new char[length];

	WideCharToMultiByte(
		CP_OEMCP,
		NULL,
		pText,
		-1,
		tempText,
		length,
		nullptr,
		nullptr);

	dest = tempText;

	delete[]tempText;
}

void StringToWstring(std::wstring& dest, std::string str)
{
	std::string temp = str;

	const auto length = MultiByteToWideChar(
		CP_ACP,
		NULL,
		static_cast<LPCSTR>(temp.c_str()),
		-1,
		nullptr,
		0
	);

	const auto pWStr = new WCHAR[length + 1];

	//__try {

		memset(pWStr, 0, length * 2 + 2);
		MultiByteToWideChar(
			CP_ACP,
			NULL,
			static_cast<LPCSTR>(temp.c_str()),
			-1,
			static_cast<LPWSTR>(pWStr),
			length
		);

		dest = pWStr;
	//}
	//__finally {
		delete[]pWStr;
	//}
}

void StringToWString(std::string& str, std::wstring& dest)
{	
	
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	dest = wstr;
}