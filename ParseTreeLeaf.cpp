#include "ParseTreeLeaf.h"
#include "Terminal.h"

namespace Parser
{
	CParseTreeLeaf::CParseTreeLeaf(CTerminal * terminal, CParsingState * state)
		: CParseTree(state), terminal(terminal)
	{}

	CParseTreeLeaf::~CParseTreeLeaf()
	{
		delete terminal;
	}

	const CTerminal * CParseTreeLeaf::GetTerminal() const
	{
		return terminal;
	}
}
