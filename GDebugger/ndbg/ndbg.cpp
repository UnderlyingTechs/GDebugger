// ndbg.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Shlwapi.h>
#include <iostream>
#include "../DebuggerCore/DebuggerService.h"
#include "../DebuggerCommand/LocalCommand.h"
#include "SysCommands.h"
#include "../DebuggerCore/DebugUtil.h"

void PreloadSystemCommands();

int main(int argc, char* argv)
{
	PreloadSystemCommands();

	TCHAR readCommand[2048];

	std::wcin.getline(readCommand, 2048);

	auto pContext = new RunningContext();

	while (readCommand)
	{
		if (wcscmp(readCommand, TEXT("exit")) == 0)
		{
			//
			// 退出命令
			//

			break;
		}
		else 
		{
			std::string commandStr;
			WCHARToString(commandStr, readCommand);
			const auto parsedCommand = CommandService::ParseCommand(commandStr);

			if (!parsedCommand->HasError())
			{
				auto findedCommand = CommandService::GetInstance()->FindCommand(*parsedCommand->GetName());
				if (nullptr != findedCommand)
				{
					pContext->pCommandInfo = parsedCommand;
					findedCommand->RunCommand(pContext);
				}
				else
				{
					std::wcout << TEXT("unknow command : ") << readCommand << std::endl;
				}
			}
			else
			{
				std::wcout << TEXT("command parse error: ") << std::endl;

				const auto errors = parsedCommand->GetErrors();
				auto errorIterator = errors.begin();

				while (errorIterator != errors.end())
				{
					const auto error = *errorIterator++;
					std::wcout << TEXT("(") <<
						error->GetRow() << TEXT(",") <<
						error->GetColumn() << TEXT(") -> ");

					std::cout << error->GetError()->c_str() << std::endl;
				}
			}
		}

		std::wcin.getline(readCommand, 2048);
	}

	//
	// OK ，运行到此时说明调试成功，此时继续开始读取用户输入 
	//
    return 0;
}

void PreloadSystemCommands()
{
	auto commandService = CommandService::GetInstance();

	const auto debugProcessCommand = new DebugProcessCommand;
	commandService->RegisterDebugCommand(debugProcessCommand);
}