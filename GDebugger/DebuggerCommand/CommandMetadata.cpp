#include "LocalCommand.h"

CommandMetadata::~CommandMetadata()
{
	auto parameterIterator = this->parameters.begin();
	while (parameterIterator != this->parameters.end())
	{
		const auto parameter = *parameterIterator++;
		delete parameter;
	}
}
