#include "DebuggerService.h"

DWORD DebuggerService::Attach(HANDLE, DebuggerProcess*)
{
	return 0;
}

DWORD DebuggerService::Debug(TCHAR*, DebuggerProcess*)
{
	return 0;
}

void DebuggerService::Initialzie()
{
	
}


