#pragma once
#include "SyntaxRule.h"
#include "Enums.h"

namespace GrammarSymbols
{
	class CGroup;

	class CHelperSyntaxRule :
		public CSyntaxRule
	{
		const CGroup* originalRule = nullptr;
	public:
		virtual ~CHelperSyntaxRule();
		CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CDefinitionList& origin);
		CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CMetaIdentifier& helperIdentifier2, Option option);
		void WriteTo(_STD ostream& os) const override;
		CGroup* CreateReplacement() const;
	};
}