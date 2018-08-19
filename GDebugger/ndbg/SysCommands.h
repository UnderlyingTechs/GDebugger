#pragma once
#include "../DebuggerCommand/LocalCommand.h"

/*
 * 定义基本的命令类型
 */

/**
 * 启动调试进程命令，该命令可以由 debug，db，d等命令，以文件路径作为参数启动
 * 调试进程
 */
class DebugProcessCommand : public DebugCommand
{
public:
	DebugProcessCommand();
	~DebugProcessCommand();

	void RunCommand() override;
};