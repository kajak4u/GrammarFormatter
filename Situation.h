#pragma once
#include <iostream>
#include "ShortDefinition.h"

namespace GrammarSymbols
{
	class CDefinedGrammarSymbol;
	class CShortDefinition;
}

namespace Parser
{
	using namespace ::GrammarSymbols;
	//a single syntax situation - a simple rule with marked parser position
	struct CSituation
	{
		//rule's result
		const CDefinedGrammarSymbol* result;
		//result's derivation
		const CShortDefinition* def;
		//current parser position
		CShortDefinition::const_iterator pos;
		//terminals allowed to go forward
		CTerminal* allowed;
		//constructor
		CSituation(const CDefinedGrammarSymbol* result, const CShortDefinition* def, CShortDefinition::const_iterator pos, CTerminal* allowed);
		//constructor with pos set at definition's begin
		CSituation(const CDefinedGrammarSymbol* result, const CShortDefinition* def, CTerminal* allowed = nullptr);
		//returns its copy but with position moved forward
		CSituation nextPos() const;

		//boolean relation operators
		bool operator<(const CSituation& other) const;
		bool operator==(const CSituation& other) const;
	};
	//output stream's operator
	_STD ostream& operator<<(_STD ostream& os, const CSituation& situation);

	using CSituations = MySet<CSituation>;
}