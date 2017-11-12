#pragma once

#include <vector>
#include "ParserAction.h"
#include "Syntax.h"
#include <unordered_set>
#include "MetaIdentifier.h"

class CDefinitionList;
class CDefinition;
class CTerm;

class CParsingTable : public std::vector<CParserAction*>
{
	typedef std::unordered_set<int> RulesList;
	typedef std::map<CMetaIdentifier, std::vector<std::pair<int, const CDefinitionList*>>> RulesMap;
	RulesList Parse(const RulesList& currentRules, const CDefinition* definition);
	RulesList Parse(const RulesList& currentRules, const CTerm* term);

	std::map<CMetaIdentifier, CParserAction*> IdentifiersInMemory;
	RulesMap rulesMap;

	void ParseIdentifier(const CMetaIdentifier& identifier);
public:
	CParsingTable();
	CParsingTable(const CParsingTable& other);
	CParsingTable(CParsingTable&&) = default;
	void Parse(const CSyntax& grammar);
	~CParsingTable();
};

