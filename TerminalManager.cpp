#include "TerminalManager.h"

namespace GrammarSymbols
{
	//terminal additional memory output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CTerminalManagerItem&)
	{
		return os;
	}
}