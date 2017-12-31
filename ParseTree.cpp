#include "ParseTree.h"

namespace Parser
{

	CParseTree::CParseTree(CParsingState * state)
		: state(state)
	{}

	CParseTree::~CParseTree()
	{
	}

	CParsingState * const CParseTree::GetState() const
	{
		return state;
	}

	void CParseTree::SetState(CParsingState * newState)
	{
		state = newState;
	}

}