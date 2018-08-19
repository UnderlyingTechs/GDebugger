#include "stdafx.h"
#include "SysCommands.h"
#include <iostream>

void DebugProcessCommand::RunCommand()
{
	std::cout << "Test run debugProcessCommand" << std::endl;
}

DebugProcessCommand::DebugProcessCommand()
{
	auto metadata = new CommandMetadata();
	metadata->AddName(std::string("debug"));
	metadata->AddName(std::string("db"));
	metadata->AddName(std::string("d"));

	auto parameter = new ParameterDescription(std::string("The full path of the debug process file."));
	parameter->AddName(std::string("-filename"));
	parameter->AddName(std::string("-file"));
	parameter->AddName(std::string("-f"));

	metadata->AddParameterDescription(parameter);
	this->SetCommandMetadata(metadata);
}

DebugProcessCommand::~DebugProcessCommand()
= default;


