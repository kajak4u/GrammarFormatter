#include "DefinitionList.h"
#include "main.h"
#include "Definition.h"


CDefinitionList::CDefinitionList()
{
}


CDefinitionList::~CDefinitionList()
{
	for (CDefinition*& def : *this)
		delete def;
}

std::istream& CDefinitionList::ReadFrom(std::istream& is)
{
	do
	{
		this->push_back(new CDefinition(is));
		skipWhiteChars(is);
		//usun symbol ze strumienia tylko jesli jest to separator
		if (GetSymbol(is, false) == SymbolSeparator)
			GetSymbol(is, true);
		else
			break;
	} while (true);
	return is;
}

void CDefinitionList::WriteTo(std::ostream & os) const
{
	bool first = true;
	for (const CDefinition* def : *this)
		os << (first ? first = false, "" : "\n\t| ") << *def;
}

std::ostream & operator<<(std::ostream & os, const CDefinitionList & list)
{
	list.WriteTo(os);
	return os;
}
