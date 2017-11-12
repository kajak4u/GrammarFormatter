#pragma once

#include <vector>
#include "ParserAction.h"
#include "Syntax.h"
#include <unordered_set>

class CDefinition;
class CTerm;

class CParsingTable : public std::vector<CParserAction*>
{
	typedef std::unordered_set<int> RulesList;
	RulesList Parse(const RulesList& currentRules, const CDefinition* definition);
	RulesList Parse(const RulesList& currentRules, const CTerm* term);
public:
	CParsingTable();
	CParsingTable(const CParsingTable& other);
	CParsingTable(CParsingTable&&) = default;
	void Parse(const CSyntax& grammar);
	~CParsingTable();
};

