#include "LocalCommand.h"

ParsedParameter::ParsedParameter()
= default;


ParsedParameter::~ParsedParameter()
{
	const auto pName = this->GetName();
	if (pName != nullptr)
	{
		delete pName;
		this->SetName(nullptr);
	}

	auto parameters = this->GetValues();
	auto parameterIterator = parameters.begin();
	while (parameterIterator != parameters.end())
	{
		const auto parameter = *parameterIterator++;
		delete parameter;
	}
}
