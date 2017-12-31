#pragma once
#include "SyntaxRule.h"
#include "Enums.h"

namespace GrammarSymbols
{
	class CGroup;

	//helper syntax rule - additional rules introduced to syntax to eliminate groups from syntax
	class CHelperSyntaxRule : public CSyntaxRule
	{
	public:
		//create rule using given identifier and definition list
		CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CDefinitionList& origin);
		//create second rule for repeat/optional group using new identifier and primary identifier
		CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CMetaIdentifier& helperIdentifier2, GroupType option);
		//destructor
		virtual ~CHelperSyntaxRule();
	};
}