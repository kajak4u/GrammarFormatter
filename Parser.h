#pragma once

class CSyntax;
class CTerminal;
#include "Definition.h"
#include "Terminal.h"

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
	CSituation nextPos() const
	{
		return CSituation(def, std::next(pos), allowed);
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
std::ostream& operator<<(std::ostream& os, const CSituation& situation);

class CParser
{
	using CSituations = MySet<CSituation>;
	CSituations Closure(const CSituations& situations);
	CSituations Goto(const CSituations& situations, const CPrimary* symbol);
public:
	CParser();
	void Parse(const CSyntax& grammar);
	~CParser();
};
