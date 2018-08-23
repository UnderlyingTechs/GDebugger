#include "LocalCommand.h"

String* ParsedCommand::FindParameterValue(const String& name, const Size index) const
{
	const auto parameters = this->GetParameters();
	auto parameterIterator = parameters.begin();
	while (parameterIterator != parameters.end())
	{
		const auto parameter = *parameterIterator++;
		if (*parameter->GetName() == name)
		{
			const auto values = parameter->GetValues();
			if (nullptr == values || values->size() <= index)
			{
				return nullptr;
			}

			return (*values)[index];
		}
	}

	return nullptr;
}

StringList ParsedCommand::FindParameterValues(const String& name) const
{
	const auto parameters = this->GetParameters();
	auto parameterIterator = parameters.begin();
	while (parameterIterator != parameters.end())
	{
		const auto parameter = *parameterIterator++;
		if (*parameter->GetName() == name)
		{
			return parameter->GetValues();
		}
	}

	return nullptr;
}