#pragma once

#include <vector>
#include <map>
#include "Syntax.h"
#include "Parser.h"
#include "main.h"

namespace GrammarSymbols {
	class CSyntax;
	class CTerminal;
	class CMetaIdentifier;
}
class CParsingState;

class CAction
{
public:
	virtual void Perform(CParser& parser) = 0;
};
class CGoto
{
	CParsingState* newState;
public:
	CGoto(CParsingState* newState);
	void Perform(CParser& parser);
};

class CParsingState
{
public:
	CSituations* situations;
	_STD map<const CTerminal*, CAction*, CompareObjects<CTerminal>> actions;
	_STD map<const CMetaIdentifier*, CGoto*, CompareObjects<CMetaIdentifier>> gotos;
	CParsingState(CSituations* situations)
		: situations(situations)
	{}
#ifdef _DEBUG
	int id;
	CParsingState(int id, CSituations* situations)
		: id(id), situations(situations)
	{}
#endif
};

class CShiftAction : public CAction
{
	CParsingState* newState;
public:
	CShiftAction(CParsingState* newState);
	virtual void Perform(CParser & parser) override;
};
class CAcceptAction : public CAction
{
public:
	virtual void Perform(CParser & parser) override;
};
class CReduceAction : public CAction
{
	const CMetaIdentifier* result;
	const CShortDefinition* definition;
public:
	CReduceAction(const CMetaIdentifier* result, const CShortDefinition* definition);
	virtual void Perform(CParser & parser) override;
};

class CParsingTable : public std::vector<CParsingState*>
{
	CSituations Closure(const CSituations& situations);
	CSituations Goto(const CSituations& situations, const CPrimary* symbol);
public:
	CParsingTable(const GrammarSymbols::CSyntax& grammar);
	CParsingState* AddOrGet(CSituations* situations);
};
_STD ostream& operator<<(_STD ostream& os, const CParsingTable& table);