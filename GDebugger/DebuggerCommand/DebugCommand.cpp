#include "LocalCommand.h"
#include <iostream>

void DebugCommand::RunCommand(RunningContext* pContext)
{
	/**
	 * 检查所有必须参数是否全部存在
	 */

	const auto parameters = this->GetCommandMetadata()->GetParameterDescriptions();
	auto parameterDescriptionIterator = parameters.begin();
	while (parameterDescriptionIterator != parameters.end())
	{
		const auto parameterDescription = *parameterDescriptionIterator++;
		if (parameterDescription->IsOptional() == FALSE)
		{
			const auto parsedParameters = pContext->pCommandInfo->GetParameters();
			auto parsedParameterIterator = parsedParameters.begin();
			BOOL parameterValueFound = FALSE;

			while (parsedParameterIterator != parsedParameters.end())
			{
				const auto parsedParameter = *parsedParameterIterator++;
				if (parameterDescription->IsCommandNameMatch(*parsedParameter->GetName()))
				{
					parameterValueFound = TRUE;
					break;
				}
			}

			if (FALSE == parameterValueFound)
			{
				/**
				 * 必须的参数没有设置，报错，直接返回.
				 */

				std::cout << "parameter ";
				auto parameterNames = parameterDescription->GetNames();
				for (size_t i = 0; i < parameterNames.size(); i++)
				{
					if (i > 0)
					{
						std::cout << "/";
					}

					std::cout << *parameterNames[i];
				}

				std::cout << " must be specified." << std::endl;
				return;
			}
		}
	}

	// OK 所有有效参数检查通过，开始执行命令
	this->RunCommandCore(pContext);
}

