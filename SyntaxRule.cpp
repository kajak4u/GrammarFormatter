#include "SyntaxRule.h"
#include "main.h"

using namespace std;

CSyntaxRule::CSyntaxRule()
{
}

CSyntaxRule::CSyntaxRule(std::istream & is)
{
	ReadFrom(is);
}


CSyntaxRule::~CSyntaxRule()
{
}

std::istream & CSyntaxRule::ReadFrom(std::istream & is)
{
	identifier.ReadFrom(is);
	if (GetSymbol(is, true) != SymbolDefining)
		throw invalid_argument(string() + " expected defining symbol '=' after identifier '" + identifier.GetName() + "'.");
	definitionList.ReadFrom(is);
	if (GetSymbol(is, true) != SymbolTerminator)
		throw invalid_argument(string() + " expected terminator symbol ';' or '.' after definition list for identifier '" + identifier.GetName() + "'.");
	return is;
}
