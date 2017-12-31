#include "ParseTreeItem.h"

namespace Parser
{

	CParseTreeItem::CParseTreeItem(CParsingState * state)
		: state(state)
	{}

	CParseTreeItem::~CParseTreeItem()
	{
	}

	CParsingState * const CParseTreeItem::GetState() const
	{
		return state;
	}

	void CParseTreeItem::SetState(CParsingState * newState)
	{
		state = newState;
	}

}