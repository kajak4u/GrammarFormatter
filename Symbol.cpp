#include "Symbol.h"

CSymbol::~CSymbol()
{
	for (auto& rule : rules)
		delete rule;
}
