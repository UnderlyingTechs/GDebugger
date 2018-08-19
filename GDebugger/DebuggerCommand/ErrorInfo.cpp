#include "LocalCommand.h"

ErrorInfo::~ErrorInfo()
{
	const auto pError = this->GetError();
	if (nullptr != pError)
	{
		delete pError;
		this->pError = nullptr;
	}
}
