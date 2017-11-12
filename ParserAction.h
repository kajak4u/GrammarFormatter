#pragma once

#include <unordered_map>
#include <unordered_set>

class CTerminal;
class CMetaIdentifier;

struct CParserAction
{
	typedef int SelectionRule, GotoRule, ReductionRule;
	std::unordered_map<const CTerminal*, SelectionRule>* actions = nullptr;
	std::unordered_map<const CMetaIdentifier*, GotoRule>* gotos = nullptr;
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