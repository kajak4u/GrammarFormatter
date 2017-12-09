#pragma once

class CSyntax;
class CTerminal;
#include "Definition.h"

struct CSituation
{
	const CShortDefinition* def;
	CShortDefinition::const_iterator pos;
	CTerminal* allowed;
	CSituation(const CShortDefinition* def, CShortDefinition::const_iterator pos, CTerminal* allowed)
		: def(def), pos(pos), allowed(allowed)
	{}
	CSituation(const CShortDefinition* def, CTerminal* allowed=nullptr)
		: CSituation(def, def->begin(), allowed)
	{}

	bool operator<(const CSituation& other) const
	{
		return def != other.def ? def < other.def
			: pos != other.pos ? pos < other.pos
			: allowed < other.allowed;
	}
};

class CParser
{
	using CSituations = MySet<CSituation>;
	CSituations Closure(const CSituations& situations);
public:
	CParser();
	void Parse(const CSyntax& grammar);
	~CParser();
};

