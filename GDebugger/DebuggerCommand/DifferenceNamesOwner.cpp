#include "LocalCommand.h"
#include "../DebuggerCore/DebugUtil.h"


BOOL DifferenceNamesOwner::IsCommandNameMatch(WCHAR* name) const
{
	std::string commandName;
	WCHARToString(commandName, name);
	return this->IsCommandNameMatch(commandName);
}


BOOL DifferenceNamesOwner::IsCommandNameMatch(const std::string& name) const
{
	auto nameIterator = this->names.begin();
	while (nameIterator != this->names.end())
	{
		const auto currentName = *nameIterator++;
		if (*currentName == name)
		{
			return TRUE;
		}
	}

	return FALSE;
}