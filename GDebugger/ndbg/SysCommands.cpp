#include "stdafx.h"
#include "SysCommands.h"
#include <iostream>
#include "../DebuggerCommand/LocalCommand.h"
#include "../DebuggerCore/DebugUtil.h"

void DebugProcessCommand::RunCommandCore(RunningContext* pContext)
{
	const auto commandService = CommandService::GetInstance();

	if (commandService->GetDebuggerProcess() != nullptr)
	{
		std::wcout << TEXT("error: Debug process is existed.") << std::endl;
		return;
	}

	const auto debuggerService = commandService->GetDebuggerService();

	const auto parameterValue = pContext->pCommandInfo->FindParameterValue(String("-filename"));

	if (nullptr == parameterValue)
	{
		std::wcout << TEXT("error: no filename.") << std::endl;
		return;
	}

	const auto pFileName = parameterValue;

	DebuggerProcess* process;

	std::wstring dest;
	StringToWString(*pFileName, dest);
	debuggerService->DebugProcess(dest.c_str(), process);
}

DebugProcessCommand::DebugProcessCommand()
{
	auto metadata = new CommandMetadata;
	metadata->AddName(new std::string("debug"));
	metadata->AddName(new std::string("db"));
	metadata->AddName(new std::string("d"));

	auto parameter = new ParameterDescription(std::string("-filename"), 
		std::string("The full path of the debug process file."), 
		FALSE);

	parameter->AddName(new std::string("-file"));
	parameter->AddName(new std::string("-f"));

	metadata->AddParameterDescription(parameter);
	this->SetCommandMetadata(metadata);
}

DebugProcessCommand::~DebugProcessCommand()
= default;


