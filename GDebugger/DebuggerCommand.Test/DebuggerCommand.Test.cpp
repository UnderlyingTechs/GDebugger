// DebuggerCommand.Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../DebuggerCommand/LocalCommand.h"
#include <cassert>

class SampleCommand : public DebugCommand
{
public:
	void RunCommand() override;

	SampleCommand();
	~SampleCommand();
};

SampleCommand::SampleCommand()
{
	auto pMetadata = new CommandMetadata;
	pMetadata->AddCommandName(new std::string("test1"));
	pMetadata->AddParameterDescription(
		ParameterDescription::Create("name1", "desc1")
	);

	this->SetCommandMetadata(pMetadata);
}

SampleCommand::~SampleCommand()
{
}

void SampleCommand::RunCommand()
{
	
}


int main()
{
	/**
	 * 测试初始化状态
	 */
	assert(CommandService::GetInstance()->GetCommands().size() == 0);
	assert(CommandService::GetInstance()->GetDebuggerProcess() == nullptr);
	assert(CommandService::GetInstance()->GetDebuggerService() == nullptr);

	const auto command = new SampleCommand;
	CommandService::GetInstance()->RegisterDebugCommand(command);
	assert(CommandService::GetInstance()->GetCommands().size() == 1);

	const auto findedCommand = CommandService::GetInstance()->FindCommand("test1");
	assert(findedCommand != nullptr);

    return 0;
}

