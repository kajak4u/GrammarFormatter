#pragma once

#include <map>
#include "Terminal.h"
#include "MetaIdentifier.h"

class CTerminal;
class CMetaIdentifier;

struct CParserAction
{
public:
	typedef int SelectionRule, GotoRule, ReductionRule;
	typedef std::map<const CTerminal*, SelectionRule, CTerminal::ComparePointers> ActionTable;
	typedef std::map<const CMetaIdentifier*, GotoRule, CMetaIdentifier::ComparePointers> GotoTable;
//private:
	ActionTable* actions = nullptr;
	GotoTable* gotos = nullptr;
	ReductionRule reductionRule = -1;
public:
	CParserAction() {}
	CParserAction(int reductionRule)
		: reductionRule(reductionRule)
	{}
	void AddGoto(const CMetaIdentifier* id, GotoRule nextRule);
	GotoRule GetGoto(const CMetaIdentifier* id) const;
	void AddShiftAction(const CTerminal* id, SelectionRule nextRule);
	SelectionRule GetShiftAction(const CTerminal* id) const;
};