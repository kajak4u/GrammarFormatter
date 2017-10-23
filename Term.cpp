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
		hasException = true;
		GetSymbol(is, true);
		exception.ReadFrom(is);
	}
	return is;
}

void CTerm::WriteTo(std::ostream & os) const
{
	os << factor;
	if (hasException)
		os << " - " << exception;
}

std::ostream & operator<<(std::ostream & os, const CTerm & term)
{
	term.WriteTo(os);
	return os;
}
