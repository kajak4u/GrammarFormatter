#pragma once

#include "ShortDefinition.h"
#include "Terminal.h"
#include <iostream>

namespace GrammarSymbols {
	class CSyntax;
	class CTerminal;
	class CMetaIdentifier;
	class CShortDefinition;
	class CPrimary;
}
using namespace GrammarSymbols;

class CParsingState;

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

class CDrzewo
{
	CParsingState* state;
public:
	CParsingState* const GetState() const { return state; }
	void SetState(CParsingState* newState) { state = newState; }
	CDrzewo(CParsingState* state)
		: state(state)
	{}
	virtual ~CDrzewo() {}
};
class CNode : public CDrzewo
{
public:
	using SubTree = std::vector<std::pair<CPrimary*, CDrzewo*>>;
private:
	const CMetaIdentifier* identifier;
	SubTree subtree;
public:
	CNode(const CMetaIdentifier* identifier, const SubTree& subtree, CParsingState* state)
		: CDrzewo(state), identifier(identifier), subtree(subtree)
	{}
	virtual ~CNode()
	{
		for (auto& elem : subtree)
			delete elem.second;
	}
	const CMetaIdentifier* GetIdentifier() const;
	const SubTree& getSubTree() const;
};
class CLeaf : public CDrzewo
{
	CTerminal* terminal;
public:
	CLeaf(CTerminal* terminal, CParsingState* state)
		: CDrzewo(state), terminal(terminal)
	{}
	virtual ~CLeaf() { delete terminal; }
	const CTerminal* GetTerminal() const;
};

class CParser
{
	CParsingState* currentState;
	bool accepted = false;
	std::vector<CDrzewo*> stack;
	CTerminal* currentTerminal = nullptr;
public:
	void ChangeStateTo(CParsingState* state);
	void AddStateToStack(CParsingState* newState);
	void Accept();
	bool Accepted() const;
	void Reduce(const CMetaIdentifier*, const CShortDefinition*);

	void Process(_STD istream& file);

	CParser(CParsingState* startState);
	~CParser();
};

