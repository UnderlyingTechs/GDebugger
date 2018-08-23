#include "LocalCommand.h"
#include <iostream>

void DebugCommand::RunCommand(RunningContext* pContext)
{
	/**
	 * ������б�������Ƿ�ȫ������
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
				 * ����Ĳ���û�����ã�����ֱ�ӷ���.
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

	// OK ������Ч�������ͨ������ʼִ������
	this->RunCommandCore(pContext);
}

