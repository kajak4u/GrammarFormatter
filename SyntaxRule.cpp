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

const CMetaIdentifier & CSyntaxRule::GetIdentifier() const
{
	return identifier;
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

void CSyntaxRule::WriteTo(std::ostream & os) const
{
	os << identifier << " = " << definitionList << ";" << endl;
}

void CSyntaxRule::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	definitionList.ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CSyntaxRule & rule)
{
	rule.WriteTo(os);
	return os;
}
