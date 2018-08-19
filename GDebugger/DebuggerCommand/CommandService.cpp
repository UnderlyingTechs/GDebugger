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


DebugCommand* CommandService::FindCommand(std::list<std::string> names)
{
	auto nameInterator = names.begin();

	while (nameInterator != names.end())
	{
		const auto name = *nameInterator++;
		const auto pCommand = this->FindCommand(name);

		if (nullptr != pCommand)
		{
			return pCommand;
		}
	}

	return nullptr;
}


void CommandService::RegisterDebugCommand(DebugCommand* pInputCommand)
{
	const auto pCommand = this->FindCommand(pInputCommand->GetCommandMetadata()->GetNames());

	if (pCommand != nullptr)
	{
		/**
		 * 当前命令已经存在，目前直接忽略，后续考虑抛出异常
		 */

		return;
	}

	this->commands.push_back(pInputCommand);
}

ParsedCommand* CommandService::ParseCommand(String& content)
{
	return nullptr;
}


//
//BOOL CommandMetadata::IsCommandNameMatch(TCHAR* name) const
//{
//	const auto commandName = std::string(name);
//	return this->IsCommandNameMatch(commandName);
//}


