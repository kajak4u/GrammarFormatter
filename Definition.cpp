#include "Definition.h"
#include "Enums.h"
#include "main.h"
#include "Term.h"


CDefinition::CDefinition()
{
}

CDefinition::CDefinition(std::istream & is)
	: CDefinition()
{
	ReadFrom(is);
}


CDefinition::~CDefinition()
{
	for (CTerm*& term : *this)
		delete term;
}

std::istream& CDefinition::ReadFrom(std::istream& is)
{
	do
	{
		this->push_back(new CTerm(is));
		skipWhiteChars(is);
		//usun symbol ze strumienia tylko jesli jest to separator
		if (GetSymbol(is, false) == SymbolConcatenate)
			GetSymbol(is, true);
		else
			break;
	} while (true);
	return is;
}