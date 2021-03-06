// SimpleExceptionMaker.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

void ThrowDivByZeroExceptionNoTryCatch();
void ThrowDivByZeroExceptionWithTryCatch();
void ThrowDivByZeroExpcetionWithCplusplusTryCatch();

int main()
{
	//ThrowDivByZeroExceptionWithTryCatch();
	//ThrowDivByZeroExceptionNoTryCatch();
	ThrowDivByZeroExpcetionWithCplusplusTryCatch();
	return 0;
}

void ThrowDivByZeroExceptionNoTryCatch()
{
	int a = 0;
	int b = 10 / a;
	printf("%d", b);
}

void ThrowDivByZeroExceptionWithTryCatch()
{
	OutputDebugString(TEXT("throw exceptions..."));

	__try
	{
		int a = 0;
		int b = 10 / a;
		//printf("%d", b);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		OutputDebugString(TEXT("catch exception"));
	}
}

void ThrowDivByZeroExpcetionWithCplusplusTryCatch()
{
	OutputDebugString(TEXT("throw exceptions..."));

	try
	{
		throw 9;
	}
	catch (int ex)
	{
		OutputDebugString(TEXT("Entered exception handler."));
	}
}

