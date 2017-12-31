#pragma once
#include "Action.h"

namespace GrammarSymbols
{
	class CDefinedGrammarSymbol;
	class CShortDefinition;
}

namespace Parser
{
	using namespace ::GrammarSymbols;

	//represents parser's action 'REDUCE by [result] --> [definition]' 
	class CReduceAction : public CAction
	{
		//reduction result
		const CDefinedGrammarSymbol* result;
		//result's derivation
		const CShortDefinition* definition;
	public:
		CReduceAction(const CDefinedGrammarSymbol* result, const CShortDefinition* definition);
		virtual void Perform(CParser & parser) override;
	};
}
