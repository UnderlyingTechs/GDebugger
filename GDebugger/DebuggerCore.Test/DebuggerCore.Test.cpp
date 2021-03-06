// DebuggerCore.Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <Shlwapi.h>

void OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO*);

void OnThreadCreated(const CREATE_THREAD_DEBUG_INFO*);

void OnException(const EXCEPTION_DEBUG_INFO*);

void OnProcessExited(const EXIT_PROCESS_DEBUG_INFO*);

void OnThreadExited(const EXIT_THREAD_DEBUG_INFO*);

void OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO*, PROCESS_INFORMATION*);

void OnRipEvent(const RIP_INFO*, PROCESS_INFORMATION*);

void OnDllLoaded(const LOAD_DLL_DEBUG_INFO*);

void OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO*);

int main()
{
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);

	PROCESS_INFORMATION processInfo = { 0 };

	TCHAR exeFullPath[MAX_PATH];

	if (!GetModuleFileName(NULL, exeFullPath, MAX_PATH))
	{
		std::wcout << TEXT("Read sample app failed, error : ") << GetLastError() << std::endl;
		return -1;
	}

	if (!PathRemoveFileSpec(exeFullPath))
	{
		std::wcout << TEXT("Read sample app failed 1, error : ") << GetLastError() << std::endl;
		return -1;
	}

	if (!PathAppend(exeFullPath, TEXT("SimpleExceptionMaker.exe")))
	{
		std::wcout << TEXT("Read sample app failed 2, error : ") << GetLastError() << std::endl;
		return -1;
	}
	
	if (CreateProcess(exeFullPath, 
		NULL,
		NULL,
		NULL,
		FALSE,
		DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&startupInfo,
		&processInfo) == FALSE)
	{
		std::wcout << TEXT("CreateProcess failed, error : ") << GetLastError() << std::endl;
		return -1;
	}

	std::wcout << TEXT("Create process success.") << std::endl;

	BOOL        waitEvent = TRUE;
	DEBUG_EVENT debugEvent;
	DWORD       debugFlag = DBG_CONTINUE;

	INT         exceptionNum = 0;

	while (TRUE == waitEvent && 
		   WaitForDebugEvent(&debugEvent, INFINITE))
	{
		debugFlag = DBG_CONTINUE;

		switch (debugEvent.dwDebugEventCode)
		{
			case CREATE_PROCESS_DEBUG_EVENT:
				OnProcessCreated(&debugEvent.u.CreateProcessInfo);
				break;
			case CREATE_THREAD_DEBUG_EVENT:
				OnThreadCreated(&debugEvent.u.CreateThread);
				break;
			case EXCEPTION_DEBUG_EVENT:

				//if (0 == exceptionNum)
				//{
					//debugFlag = DBG_EXCEPTION_NOT_HANDLED;	
				//}
				//else
				//{
					debugFlag = DBG_CONTINUE;
				//}

				exceptionNum++;
				OnException(&debugEvent.u.Exception);
				break;
			case EXIT_PROCESS_DEBUG_EVENT:
				OnProcessExited(&debugEvent.u.ExitProcess);
				break;
			case EXIT_THREAD_DEBUG_EVENT:
				OnThreadExited(&debugEvent.u.ExitThread);
				break;
			case LOAD_DLL_DEBUG_EVENT:
				OnDllLoaded(&debugEvent.u.LoadDll);
				break;
			case UNLOAD_DLL_DEBUG_EVENT:
				OnDllUnloaded(&debugEvent.u.UnloadDll);
				break;
			case OUTPUT_DEBUG_STRING_EVENT:
				OnOutputDebugString(&debugEvent.u.DebugString, &processInfo);
				break;
			case RIP_EVENT:
				OnRipEvent(&debugEvent.u.RipInfo, &processInfo);
				break;
			default:
				std::wcout << TEXT("unknow debug event : ") << std::endl;
				break;
		}

		if (TRUE == waitEvent)
		{
			ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, debugFlag);
		}
		else
		{
			break;
		}
	}

	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

    return 0;
}

void OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO* pCreateProcessDebugInfo)
{
	CloseHandle(pCreateProcessDebugInfo->hFile);
	CloseHandle(pCreateProcessDebugInfo->hThread);
	CloseHandle(pCreateProcessDebugInfo->hProcess);

	std::wcout << TEXT("process created") << std::endl;
}

void OnThreadCreated(const CREATE_THREAD_DEBUG_INFO* pCreateThreadDebugInfo)
{
	CloseHandle(pCreateThreadDebugInfo->hThread);

	std::wcout << TEXT("thread created") << std::endl;
}


void OnException(const EXCEPTION_DEBUG_INFO* pExceptionDebugInfo)
{
	if (pExceptionDebugInfo->dwFirstChance == TRUE)
	{
		std::wcout << TEXT("First chance.");
	}
	else
	{
		std::wcout << TEXT("Second chance.");
	}

	std::wcout << TEXT("Exception occured.")
		<< TEXT("Error code : ") << std::hex << std::uppercase << std::setw(8)
		<< std::setfill(L'0') << pExceptionDebugInfo->ExceptionRecord.ExceptionCode 
		<< std::dec << std::endl;
}

void OnProcessExited(const EXIT_PROCESS_DEBUG_INFO*)
{
	std::wcout << TEXT("process exited") << std::endl;
}

void OnThreadExited(const EXIT_THREAD_DEBUG_INFO*)
{
	std::wcout << TEXT("thread exited") << std::endl;
}

void OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO* pOutputDebugStringInfo, PROCESS_INFORMATION* pProcess)
{
	const auto pBuffer = static_cast<BYTE*>(malloc(pOutputDebugStringInfo->nDebugStringLength));

	SIZE_T bytes;

	if (ReadProcessMemory(pProcess->hProcess,
		pOutputDebugStringInfo->lpDebugStringData,
		pBuffer,
		pOutputDebugStringInfo->nDebugStringLength,
		&bytes) == TRUE)
	{
		//
		// 已经将内存数据读取到buffer中，确定宽字符长度
		//


		const int stringLength = MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			reinterpret_cast<LPCSTR>(pBuffer),
			pOutputDebugStringInfo->nDebugStringLength,
			nullptr,
			0);

		auto pWideString = static_cast<TCHAR*>(malloc(stringLength * sizeof(TCHAR)));

		MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			reinterpret_cast<LPCSTR>(pBuffer),
			pOutputDebugStringInfo->nDebugStringLength,
			pWideString,
			stringLength
		);

		std::wcout << TEXT("Debuggee string : ") << pWideString << std::endl;

		free(pWideString);
		free(pBuffer);
	}
}

void OnRipEvent(const RIP_INFO* pRipInfo, PROCESS_INFORMATION* pProcessInfo)
{
	std::wcout << TEXT("RIP event.") << std::endl;
}

void OnDllLoaded(const LOAD_DLL_DEBUG_INFO* pLoadDllDebugInfo)
{
	CloseHandle(pLoadDllDebugInfo->hFile);

	std::wcout << TEXT("on dll loaded") << std::endl;
}

void OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO* pUnloadedDllDebugInfo)
{
	std::wcout << TEXT("dll unloaded") << std::endl;
}
