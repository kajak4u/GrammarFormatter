#pragma once

#include "Situation.h"
#include <map>

namespace Parser
{
	class CAction;
	class CGoto;

	//represents a parser's state
	class CParsingState
	{
		//set of situations - syntax possibilities for the state
		CSituations* situations;
		//array of actions when encounter a terminal
		_STD map<const CTerminal*, CAction*, CompareObjects<CTerminal>> actions;
		//array of gotos when reduced to a symbol
		_STD map<const CDefinedGrammarSymbol*, CGoto*, CompareObjects<CDefinedGrammarSymbol>> gotos;
	public:
		//constructor
		CParsingState(CSituations* situations);
		//destructor
		~CParsingState();
		//set an action on given terminal
		void AddAction(const CTerminal*, CAction*);
		//set an goto on given symbol
		void AddGoto(const CDefinedGrammarSymbol*, CGoto*);
		//returns an action corresponding to given terminal
		CAction* GetAction(const CTerminal*) const;
		//returns a goto corresponding to given symbol
		CGoto* GetGoto(const CDefinedGrammarSymbol*) const;
		//return situations set
		const CSituations* GetSituations() const;
#ifdef _DEBUG
		//state's record number - used in debug mode for it is more convenient for human than comparing pointers
		int id;
		CParsingState(int id, CSituations* situations)
			: id(id), situations(situations)
		{}
#endif
	};
}