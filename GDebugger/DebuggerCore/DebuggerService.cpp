#include "DebuggerService.h"

DWORD DebuggerService::AttachProcess(HANDLE processId, DebuggerProcess*& ppDebuggerProcess)
{
	return 0;
}

DWORD DebuggerService::DebugProcess(const WCHAR* processFileName, DebuggerProcess*& ppDebuggerProcess)
{
	return 0;
}

void DebuggerService::Initialzie()
{
	
}

DebuggerService::DebuggerService()
= default;

DebuggerService::DebuggerService(TCHAR*)
{
	
}




