#pragma once

#include <vector>
#include "Situation.h"

namespace GrammarSymbols {
	class CSyntax;
}

namespace Parser
{
	class CParsingState;

	//parsing table - an array of parsing states
	class CParsingTable : public std::vector<CParsingState*>
	{
		//returns given situations set's closure
		CSituations Closure(const CSituations& situations);
		//returns goto for given situations set and symbol
		CSituations Goto(const CSituations& situations, const CPrimary* symbol);
	public:
		//constructor from given syntax
		CParsingTable(const GrammarSymbols::CSyntax& grammar);
		//move constructor
		CParsingTable(CParsingTable&& other);
		//destructor
		~CParsingTable();
		//creates new parsing state or returns pointer to identical one, already existing in table
		CParsingState* AddOrGet(CSituations* situations);
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CParsingTable& table);
}