#include "Term.h"
#include "main.h"

using namespace std;

CTerm::CTerm()
{
}

CTerm::CTerm(std::istream &is)
{
	ReadFrom(is);
}


CTerm::~CTerm()
{
}

std::istream & CTerm::ReadFrom(std::istream & is)
{
	factor.ReadFrom(is);
	if (GetSymbol(is, false) == SymbolExcept)
	{
		GetSymbol(is, true);
		exception.ReadFrom(is);
	}
	return is;
}
