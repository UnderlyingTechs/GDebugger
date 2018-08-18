#include "LocalCommand.h"
#include "../DebuggerCore/DebugUtil.h"

CommandService::CommandService()
{
	this->pDebuggerService = new DebuggerService();
	this->pDebuggerProcess = nullptr;
}

CommandService::CommandService(const CommandService& service)
{
	this->pDebuggerService = service.GetDebuggerService();
	this->pDebuggerProcess = service.GetDebuggerProcess();
}

CommandService* CommandService::instance = new CommandService();

CommandService* CommandService::GetInstance()
{
	return instance;
}

DebugCommand* CommandService::FindCommand(WCHAR* name)
{
	std::string commandName;
	WCHARToString(commandName, name);
	return this->FindCommand(commandName);
}


class DebugCommand* CommandService::FindCommand(const std::string& commandName)
{
	auto commandIterator = this->commands.begin();

	while (commandIterator != this->commands.end())
	{
		DebugCommand* command = *commandIterator++;

		if (command->GetCommandMetadata()->IsCommandNameMatch(commandName))
		{
			return command;
		}
	}

	return nullptr;
}


DebugCommand* CommandService::FindCommand(std::list<std::string*> names)
{
	auto nameInterator = names.begin();

	while (nameInterator != names.end())
	{
		const auto name = *nameInterator++;
		const auto pCommand = this->FindCommand(*name);

		if (nullptr != pCommand)
		{
			return pCommand;
		}
	}

	return nullptr;
}


void CommandService::RegisterDebugCommand(DebugCommand* pInputCommand)
{
	const auto pCommand = this->FindCommand(pInputCommand->GetCommandMetadata()->GetCommandNames());

	if (pCommand != nullptr)
	{
		/**
		 * 当前命令已经存在，目前直接忽略，后续考虑抛出异常
		 */

		return;
	}

	this->commands.push_back(pInputCommand);
}

//
//BOOL CommandMetadata::IsCommandNameMatch(TCHAR* name) const
//{
//	const auto commandName = std::string(name);
//	return this->IsCommandNameMatch(commandName);
//}

BOOL CommandMetadata::IsCommandNameMatch(WCHAR* name) const
{
	std::string commandName;
	WCHARToString(commandName, name);
	return this->IsCommandNameMatch(commandName);
}


BOOL CommandMetadata::IsCommandNameMatch(const std::string& name) const
{
	auto nameIterator = this->names.begin();
	while (nameIterator != this->names.end())
	{
		const auto currentName = *nameIterator++;
		if (*currentName == name)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CommandParameterDescription::CommandParameterDescription(std::string parameterName, std::string parameterDescription)
{
	this->parameterName = parameterName;
	this->parameterDescription = parameterDescription;
}


