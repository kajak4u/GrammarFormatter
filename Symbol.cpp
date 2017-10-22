#include "Symbol.h"

CSymbol::~CSymbol()
{
	for (auto& rule : rules)
		delete rule;
}


std::istream& CSymbol::ReadFrom(std::istream& is)
{
#pragma message("tu ciagle nic nie ma")
	return is;
}