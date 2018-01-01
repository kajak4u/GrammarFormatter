#include "Action.h"
#include "DefinedGrammarSymbol.h"
#include "Goto.h"
#include "ParsingState.h"
#include "Terminal.h"

namespace Parser
{
#ifdef _DEBUG
	CParsingState::CParsingState(int id, CSituations * situations)
		: id(id), situations(situations)
	{}
#else
	CParsingState::CParsingState(CSituations * situations)
		: situations(situations)
	{}
#endif

	CParsingState::~CParsingState()
	{
		delete situations;
		//deleted only [second] items, for [first] were copied from syntax
		for (auto& keyVal : actions)
			delete keyVal.second;
		for (auto& keyVal : gotos)
			delete keyVal.second;
	}
	void CParsingState::AddAction(const CTerminal *terminal, CAction *action)
	{
		//if there is a conflict, grammar is not SLR(1)
		if (actions.find(terminal) != actions.end())
			throw MYEXCEPTION(std::string() + "Grammar syntax error - duplicate action at " + terminal->GetValue() + ".\nGrammar is not SLR(1).", -2);
		actions[terminal] = action;
	}
	void CParsingState::AddGoto(const CDefinedGrammarSymbol *symbol, CGoto *newGoto)
	{
		//if there is a conflict, grammar is not SLR(1)
		if (gotos.find(symbol) != gotos.end())
			throw MYEXCEPTION(std::string() + "Grammar syntax error - duplicate goto at " + symbol->GetName() + ".\nGrammar is not SLR(1).", -3);
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
			throw MYEXCEPTION(std::string() + "Syntax error - unexpected terminal " + terminal->GetValue() + ".", -4);
		return iter->second;
	}
	CGoto * CParsingState::GetGoto(const CDefinedGrammarSymbol *symbol) const
	{
		auto iter = gotos.find(symbol);
		if (iter == gotos.end())
			throw MYEXCEPTION(std::string() + "Syntax error - unexpected symbol " + symbol->GetName() + ".", -5);
		return iter->second;
	}
}