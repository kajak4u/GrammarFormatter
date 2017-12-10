#pragma once
#include "ObjectManager.h"

namespace GrammarSymbols
{
	class CTerminalManagerItem
	{

	};
	_STD ostream& operator<<(_STD ostream& os, const CTerminalManagerItem&);

	using CTerminalManager = CObjectManager<CTerminalManagerItem>;
}