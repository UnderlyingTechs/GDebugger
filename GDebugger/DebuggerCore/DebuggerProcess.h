#pragma once

extern class DebuggerService;

enum ProcessDebugState
{
	/*
	 * 当前进程已经被中断
	 */
	Break,

	/*
	 * 当前进程正在运行
	 */
	Running
};

/*
* 定义一个当前获取的调试进程
*/
class DebuggerProcess
{
public:

	DebuggerProcess(DebuggerService* pDebuggerService) { this->pDebugService = pDebuggerService; }

	/*
	 * 获取当前调试进程对象的调试服务
	 */
	DebuggerService * GetService() const { return this->pDebugService; }



private:
	DebuggerService * pDebugService;
	
};