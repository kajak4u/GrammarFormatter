#include "SyntaxRule.h"
#include "Definition.h"

using namespace std;

namespace GrammarSymbols
{
	CSyntaxRule::CSyntaxRule()
	{
	}

	CSyntaxRule::CSyntaxRule(const CMetaIdentifier & identifier)
		: identifier(identifier)
	{
	}

	CSyntaxRule::CSyntaxRule(_STD istream & is)
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

	const CDefinitionList & CSyntaxRule::GetDefinitionList() const
	{
		return definitionList;
	}

	void CSyntaxRule::AddDefinition(CDefinition * definition)
	{
		definitionList.push_back(definition);
	}

	void CSyntaxRule::AddCopyDefinition(const CDefinition * definition)
	{
		definitionList.push_back(dynamic_cast<CDefinition*>(definition->Spawn(true)));
	}

	void CSyntaxRule::Simplify()
	{
		//simplify definition list
		definitionList.Simplify();
		//mark identifier as defined by all of new rules
		for (auto& definition : definitionList)
			identifier.MarkAsDefinedBy(definition);
	}

	_STD istream & CSyntaxRule::ReadFrom(_STD istream & is)
	{
		identifier.ReadFrom(is);
		if (GetSymbol(is, true) != SymbolDefining)
			throw invalid_argument(string() + " expected defining symbol '=' after identifier '" + identifier.GetName() + "'.");
		definitionList.ReadFrom(is);
		if (GetSymbol(is, true) != SymbolTerminator)
			throw invalid_argument(string() + " expected terminator symbol ';' or '.' after definition list for identifier '" + identifier.GetName() + "'.");
		return is;
	}

	void CSyntaxRule::WriteTo(_STD ostream & os) const
	{
		os << identifier << " = " << definitionList << ";" << endl;
	}

	void CSyntaxRule::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		CGrammarObject::ForEach(condition, action);
		definitionList.ForEach(condition, action);
	}

	void CSyntaxRule::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		CGrammarObject::ForEach(condition, action);
		definitionList.ForEach(condition, action);
	}
}