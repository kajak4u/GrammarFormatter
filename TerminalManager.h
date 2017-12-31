#pragma once
#include "ObjectManager.h"

namespace GrammarSymbols
{
	//additional data assigned to terminal's value
	class CTerminalManagerItem
	{

	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CTerminalManagerItem&);

	//manager class
	using CTerminalManager = CObjectManager<CTerminalManagerItem>;
}