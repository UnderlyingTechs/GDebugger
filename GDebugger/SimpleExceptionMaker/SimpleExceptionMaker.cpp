// SimpleExceptionMaker.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

void ThrowDivByZeroExceptionNoTryCatch();
void ThrowDivByZeroExceptionWithTryCatch();

int main()
{
	//ThrowDivByZeroExceptionWithTryCatch();
	ThrowDivByZeroExceptionNoTryCatch();
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

