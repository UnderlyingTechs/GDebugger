#pragma once

#include <windows.h>
#include "DebuggerProcess.h"


/*
 * 核心的调试服务类，一个DebuggerService 代表一个调试服务，用户可以基于该服务启动
 * 调试进程，或者附加到一个已经存在的进程。
 */
class __declspec(dllexport) DebuggerService
{
public:

	/*
	 * 使用默认配置初始化
	 */
	DebuggerService();

	/*
	 * 指定对应的配置文件初始化
	 */
	DebuggerService(TCHAR*);

	/*
	 * 调试服务初始化方法，必须显式调用来装载附加参数配置信息，如果用户
	 * 需要加载其他路径的配置，则在调用Initialize之前，先调用 AddParameterFilePath
	 * 或者AddServiceOptions指定配置信息，最后调用初始化。初始化完成之后部分配置
	 * 不支持更新。
	 */
	void Initialzie();

	/*
	 * 附加到一个已经存在的进程，如果成功则返回 对应的调试对象。
	 */
	DWORD AttachProcess(HANDLE, DebuggerProcess*&);

	/*
	 * 根据文件名启动一个新进程调试，如果成功则返回 对应进程的调试对象
	 */
	DWORD DebugProcess(TCHAR*, DebuggerProcess*&);
};

