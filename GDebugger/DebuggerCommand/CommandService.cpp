#include "LocalCommand.h"
#include "../DebuggerCore/DebugUtil.h"
#include <cassert>

#define ASSERT_PARSECONTEXT_STATE(pContext) assert(nullptr != (pContext)); \
assert(nullptr != (pContext)->pToken); \
assert(nullptr != (pContext)->pCommand);

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


DebugCommand* CommandService::FindCommand(std::vector<std::string> names)
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
	auto pContext   = static_cast<ParseContext*>(malloc(sizeof(ParseContext)));
	pContext->pText = &content;
	pContext->Pos   = 0;
	pContext->Row   = 0;
	pContext->Col   = 0;
	pContext->pCommand = new ParsedCommand;
	pContext->pToken   = static_cast<CommandToken*>(malloc(sizeof(CommandToken)));

	pContext->pToken->TokenType    = Unknow;
	pContext->pToken->IsTokenEated = TRUE;

	if (CommandService::ReadNextToken(pContext) == TRUE)
	{
		const auto pToken = pContext->pToken;
		if (pToken->TokenType == Identifier)
		{
			pContext->pCommand->SetName(pToken->pTokenValue);
			pToken->IsTokenEated = TRUE;

			CommandService::ReadNextToken(pContext);
			
			while (ParameterName == pToken->TokenType)
			{
				const auto pParameter = CommandService::ParseCommandParameter(pContext);
				pContext->pCommand->AddParameter(pParameter);
			}
		}
		else
		{
			pContext->pCommand->AddError(pContext->Row, pContext->Col, new std::string("Command name must be specified."));
		}
		if (End != pToken->TokenType)
		{
			// 已经解析完成，但是依然出现未预料的符号
			pContext->pCommand->AddError(pContext->Row, pContext->Col, new std::string("Unexpected command end."));
		}
	}

	const auto pCommand = pContext->pCommand;

	free(pContext->pToken);
	free(pContext);
	return pCommand;
}

ParsedParameter* CommandService::ParseCommandParameter(ParseContext* pContext)
{
	ASSERT_PARSECONTEXT_STATE(pContext);
	assert(ParameterName == pContext->pToken->TokenType);

	auto pParameter = new ParsedParameter;
	pParameter->SetName(pContext->pToken->pTokenValue);
	pContext->pToken->IsTokenEated = TRUE;

	while (CommandService::ReadNextToken(pContext) && 
		(StringValue == pContext->pToken->TokenType || Identifier == pContext->pToken->TokenType))
	{
		pParameter->AddValue(pContext->pToken->pTokenValue);
		pContext->pToken->IsTokenEated = TRUE;
	}

	return pParameter;
}

BOOL CommandService::ReadNextToken(ParseContext* pContext)
{
	ASSERT_PARSECONTEXT_STATE(pContext);
	assert(pContext->pToken->IsTokenEated);

	while (pContext->Pos < (*pContext->pText).size())
	{
		const auto ch = (*pContext->pText)[pContext->Pos];
		if ('-' == ch)
		{
			// 当前符号是一个参数名
			return CommandService::ReadParameterNameToken(pContext);
		}
		else if ('"' == ch)
		{
			return CommandService::ReadStringToken(pContext);
		}
		else if ('\r' == ch)
		{
			if ('\n' == ch)
			{
				pContext->Pos++;
				pContext->Row++;
				pContext->Col = 0;
			}
		}
		else if ('\n' == ch)
		{
			pContext->Row++; 
			pContext->Col = 0;
		}
		else if ('\t' != ch && ' ' != ch)
		{
			return CommandService::ReadIdentifierToken(pContext);
		}

		pContext->Pos++;
		pContext->Col++;
	}

	pContext->pToken->TokenType = End;
	return FALSE;
}

BOOL CommandService::ReadIdentifierToken(ParseContext* pContext)
{
	ASSERT_PARSECONTEXT_STATE(pContext);
	assert(pContext->Pos < (*pContext->pText).size());

	const auto startPos = pContext->Pos++;

	while (pContext->Pos < (*pContext->pText).size() &&
		!CommandService::IsWhitespaceChar((*pContext->pText)[pContext->Pos]))
	{
		// 如果下一个符号不是空格符，则拼接起来
		pContext->Pos++;
	}

	// OK 已经识别完整个符号
	pContext->pToken->TokenType = Identifier;
	pContext->pToken->pTokenValue = new std::string((*pContext->pText).substr(startPos, pContext->Pos - startPos));
	pContext->pToken->IsTokenEated = FALSE;

	return TRUE;
}

BOOL CommandService::ReadParameterNameToken(ParseContext* pContext)
{
	ASSERT_PARSECONTEXT_STATE(pContext);
	assert(pContext->Pos < (*pContext->pText).size());

	const auto startPos = pContext->Pos++;

	while (pContext->Pos < (*pContext->pText).size() &&
		!CommandService::IsWhitespaceChar((*pContext->pText)[pContext->Pos]))
	{
		// 如果下一个符号不是空格符，则拼接起来
		pContext->Pos++;
	}

	// OK 已经识别完整个符号
	pContext->pToken->TokenType = ParameterName;
	pContext->pToken->pTokenValue = new std::string((*pContext->pText).substr(startPos, pContext->Pos - startPos));
	pContext->pToken->IsTokenEated = FALSE;

	return TRUE;
}

BOOL CommandService::ReadStringToken(ParseContext* pContext)
{
	ASSERT_PARSECONTEXT_STATE(pContext);
	assert('"' == (*pContext->pText)[pContext->Pos]);
	assert(pContext->pToken->IsTokenEated);

	const auto pString = new std::string();

	//
	// 第一个"符号忽略，不算在整个字符串中
	//

	pContext->Pos++;

	while (pContext->Pos < (*pContext->pText).size())
	{
		auto ch = (*pContext->pText)[pContext->Pos];

		if ('\\' == ch)
		{
			// 转义字符
			if (pContext->Pos + 1 < (*pContext->pText).size())
			{
				pContext->Pos++;
				ch = (*pContext->pText)[pContext->Pos];

				if ('t' == ch)
				{
					*pString += '\t';
				}
				else if ('"' == ch)
				{
					*pString += '"';
				}
				else if ('\\' == ch)
				{
					*pString += '\\';
				}
				else if ('n' == ch)
				{
					*pString += '\n';
				}
				else if ('r' == ch)
				{
					*pString += '\r';
				}
				else
				{
					pContext->pCommand->AddError(
						pContext->Row,
						pContext->Col,
						new std::string("Unknow escape character : ") + ch);
				}
			}
		}
		else if ('"' == ch)
		{
			// OK, 字符串结束
			pContext->Pos++;
			pContext->Col++;
			break;
		}
		else
		{
			*pString += ch;
		}

		pContext->Pos++;
	}

	if ((*pContext->pText).size() == pContext->Pos && 
		(*pContext->pText)[(*pContext->pText).size() - 1] != '"')
	{
		// 说明遇到一个非自然结束
		pContext->pCommand->AddError(
			pContext->Row, 
			pContext->Col, 
			new std::string("Unexpected end of a string."));
	}

	// OK 已经识别完整个符号
	pContext->pToken->TokenType = StringValue;
	pContext->pToken->pTokenValue = pString;
	pContext->pToken->IsTokenEated = FALSE;

	return TRUE;
}

BOOL CommandService::IsWhitespaceChar(char ch)
{
	return ' ' == ch || '\r' == ch || '\t' == ch || '\n' == ch;
}





