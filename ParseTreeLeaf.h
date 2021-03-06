#pragma once

#include "ParseTreeItem.h"
namespace GrammarSymbols
{
	class CTerminal;
}

namespace Parser
{
	using namespace ::GrammarSymbols;

	//class representing a leaf in parsing tree - holding a terminal
	class CParseTreeLeaf : public CParseTreeItem
	{
		//terminal leading to that state
		CTerminal* terminal;
	public:
		//constructor
		CParseTreeLeaf(CTerminal* terminal, CParsingState* state);
		//destructor
		virtual ~CParseTreeLeaf();
		//returns associated terminal
		const CTerminal* GetTerminal() const;
	};
}

