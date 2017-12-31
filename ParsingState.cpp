#include "ParsingState.h"
#include "Terminal.h"
#include "DefinedGrammarSymbol.h"
#include "Action.h"
#include "Goto.h"
#include <string>

namespace Parser
{
	CParsingState::CParsingState(CSituations * situations)
		: situations(situations)
	{}

	CParsingState::~CParsingState()
	{
		delete situations;
		// nie usuwamy [first], bo one byly kopiowane z gramatyki
		for (auto& keyVal : actions)
		{
			delete keyVal.second;
		}
		for (auto& keyVal : gotos)
		{
			delete keyVal.second;
		}
	}
	void CParsingState::AddAction(const CTerminal *terminal, CAction *action)
	{
		if (actions.find(terminal) != actions.end())
			throw MYEXCEPTION(std::string() + "Grammar syntax error - duplicate action at " + terminal->GetValue() + ".", -1);
		actions[terminal] = action;
	}
	void CParsingState::AddGoto(const CDefinedGrammarSymbol *symbol, CGoto *newGoto)
	{
		if (gotos.find(symbol) != gotos.end())
			throw MYEXCEPTION(std::string() + "Grammar syntax error - duplicate goto at " + symbol->GetName() + ".", -1);
		gotos[symbol] = newGoto;
	}
	const CSituations * CParsingState::GetSituations() const
	{
		return situations;
	}
	CAction * CParsingState::GetAction(const CTerminal *terminal) const
	{
		auto iter = actions.find(terminal);
		if (iter == actions.end())
			throw MYEXCEPTION(std::string() + "Syntax error - unexpected terminal " + terminal->GetValue() + ".", -1);
		return iter->second;
	}
	CGoto * CParsingState::GetGoto(const CDefinedGrammarSymbol *symbol) const
	{
		auto iter = gotos.find(symbol);
		if (iter == gotos.end())
			throw MYEXCEPTION(std::string() + "Syntax error - unexpected symbol " + symbol->GetName() + ".", -1);
		return iter->second;
	}
}