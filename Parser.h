#pragma once

#include "ShortDefinition.h"
#include "Terminal.h"

namespace GrammarSymbols {
	class CSyntax;
	class CTerminal;
	class CMetaIdentifier;
}
using namespace GrammarSymbols;

struct CSituation
{
	const CMetaIdentifier* result;
	const CShortDefinition* def;
	CShortDefinition::const_iterator pos;
	CTerminal* allowed;
	CSituation(const CMetaIdentifier* result, const CShortDefinition* def, CShortDefinition::const_iterator pos, CTerminal* allowed)
		: result(result), def(def), pos(pos), allowed(allowed)
	{}
	CSituation(const CMetaIdentifier* result, const CShortDefinition* def, CTerminal* allowed=nullptr)
		: CSituation(result, def, def->begin(), allowed)
	{}
	CSituation nextPos() const
	{
		return CSituation(result, def, _STD next(pos), allowed);
	}

	bool operator<(const CSituation& other) const
	{
		return def != other.def ? def < other.def
			: pos != other.pos ? pos < other.pos
			: *allowed < *other.allowed;
	}
	bool operator==(const CSituation& other) const
	{
		return def == other.def 
			&& pos == other.pos
			&& !(*allowed < *other.allowed || *other.allowed < *allowed);
	}
};
_STD ostream& operator<<(_STD ostream& os, const CSituation& situation);

using CSituations = MySet<CSituation>;

class CParser
{
	CSituations Closure(const CSituations& situations);
	CSituations Goto(const CSituations& situations, const CPrimary* symbol);
public:
	CParser();
	void CreateParsingTable(const CSyntax& grammar);
	~CParser();
};

