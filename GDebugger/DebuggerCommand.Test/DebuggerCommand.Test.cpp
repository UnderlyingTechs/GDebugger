// DebuggerCommand.Test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../DebuggerCommand/LocalCommand.h"
#include <cassert>

#define ASSERT_COMMAND_BASE_AND_NAME(pParseCommand,name,pSize) 	assert(nullptr != (pParsedCommand)); \
assert(!(pParsedCommand)->HasError());                                       \
assert((pParsedCommand)->GetName() != nullptr);                              \
assert(*(pParsedCommand)->GetName() == name);                                 \
assert(pParsedCommand->GetParameters().size() == pSize);

/**
 * 测试命令，用来测试命令服务接口添加命令的正确性
 */
class SampleCommand : public DebugCommand
{
public:
	void RunCommandCore(RunningContext*) override;

	SampleCommand();
	~SampleCommand();
};

SampleCommand::SampleCommand()
{
	auto pMetadata = new CommandMetadata;
	pMetadata->AddName(new std::string("test1"));

	auto parameter = new ParameterDescription("paramName1", "description", TRUE);

	pMetadata->AddParameterDescription(parameter);

	this->SetCommandMetadata(pMetadata);
}

SampleCommand::~SampleCommand()
{
	delete this->GetCommandMetadata();
}

void SampleCommand::RunCommandCore(RunningContext*)
{
	
}


int main()
{
	/**
	 * note: 由于目前命令解析的行列无关紧要，所以这里目前并没有测试出错时的行列信息正确性，后续如果
	 * 需要扩展或者行列错误信息对分析错误帮助较大时再添加相关的测试。
	 */

	/**
	 * 测试初始化状态
	 */
	assert(CommandService::GetInstance()->GetCommands().size() == 0);
	assert(CommandService::GetInstance()->GetDebuggerProcess() == nullptr);
	assert(CommandService::GetInstance()->GetDebuggerService() != nullptr);

	const auto command = new SampleCommand;
	CommandService::GetInstance()->RegisterDebugCommand(command);
	assert(CommandService::GetInstance()->GetCommands().size() == 1);

	const auto findedCommand = CommandService::GetInstance()->FindCommand("test1");
	assert(findedCommand != nullptr);

	/**
	 * 测试命令解析
	 */

	// 没有参数，只有命令名称 name
	auto commandContent = std::string("name");
	auto pParsedCommand = CommandService::ParseCommand(commandContent);
	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 0);
	

	delete pParsedCommand;

	// 只有一个参数，没有参数值			name -p
	commandContent = std::string("name -p");
	pParsedCommand = CommandService::ParseCommand(commandContent);
	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 1);
	assert(*(pParsedCommand->GetParameters()[0]->GetName()) == "-p");

	delete pParsedCommand;

	// 只有一个参数，没有参数值2			name -param
	commandContent = std::string("name -param");
	pParsedCommand = CommandService::ParseCommand(commandContent);

	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 1);
	assert(*(pParsedCommand->GetParameters()[0]->GetName()) == "-param");
	delete pParsedCommand;

	// 只有一个参数, 有一个简单参数值		name -param paramValue
	commandContent = std::string("name -param paramValue");
	pParsedCommand = CommandService::ParseCommand(commandContent);

	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 1);

	auto parameter = pParsedCommand->GetParameters()[0];
	assert(*(parameter->GetName()) == "-param");

	auto values = *parameter->GetValues();
	assert(values.size() == 1);
	assert(*values[0] == "paramValue");
	delete pParsedCommand;

	// 只有一个参数，有一个字符串参数值	name -param "xxx yyy"
	commandContent = std::string("name -param \"xxx yyy\"");
	pParsedCommand = CommandService::ParseCommand(commandContent);
	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 1);
	
	parameter = pParsedCommand->GetParameters()[0];
	values = *parameter->GetValues();
	assert(*(parameter->GetName()) == "-param");
	assert(values.size() == 1);
	assert(*values[0] == "xxx yyy");
	delete pParsedCommand;

	// 有转义字符串参数值				name -param "\"\t\nxxyyzz\\"
	commandContent = std::string("name -param \"\\\"\\t\\nxxyyzz\\\\\"");
	pParsedCommand = CommandService::ParseCommand(commandContent);
	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 1);

	parameter = pParsedCommand->GetParameters()[0];
	assert(*(parameter->GetName()) == "-param");
	values = *parameter->GetValues();
	assert(values.size() == 1);
	assert(*values[0] == "\"\t\nxxyyzz\\");
	delete pParsedCommand;

	// 有多个参数值						name -param "\"\t\nxxyyzz\\" paramValue "               "
	commandContent = std::string("name -param \"\\\"\\t\\nxxyyzz\\\\\" paramValue \"               \"");
	pParsedCommand = CommandService::ParseCommand(commandContent);
	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 1);

	parameter = pParsedCommand->GetParameters()[0];
	assert(*(parameter->GetName()) == "-param");

	values = *parameter->GetValues();
	assert(values.size() == 3);
	assert(*values[0] == "\"\t\nxxyyzz\\");
	assert(*values[1] == "paramValue");
	assert(*values[2] == "               ");
	delete pParsedCommand;

	// 有多个参数						name -param1 "zz\t\"" -param2 "xxxxx" paramValue_2  
	commandContent = std::string("name -param1 \"zz\\t\\\"\" -param2 \"xxxxx\" paramValue_2");
	pParsedCommand = CommandService::ParseCommand(commandContent);
	ASSERT_COMMAND_BASE_AND_NAME(pParseCommand, "name", 2);

	parameter = pParsedCommand->GetParameters()[0];
	assert(*(parameter->GetName()) == "-param1");

	values = *parameter->GetValues();
	assert(values.size() == 1);
	assert(*values[0] == "zz\t\"");

	parameter = pParsedCommand->GetParameters()[1];
	assert(*(parameter->GetName()) == "-param2");

	values = *parameter->GetValues();
	assert(values.size() == 2);
	assert(*values[0] == "xxxxx");
	assert(*values[1] == "paramValue_2");
	delete pParsedCommand;

    return 0;
}

