#include "DefinedSymbolManager.h"
#include <iostream>
#include "Terminal.h"

namespace GrammarSymbols
{
	_STD ostream& operator<<(_STD ostream& os, const CDefinedSymbolManagerItem& item)
	{
		os << "First: {";
		for (auto& elem : item.first)
			if (elem)
				os << *elem << " ";
			else
				os << "[] ";
		os << "}\tFollow: {";
		for (auto& elem : item.follow)
			if (elem)
				os << *elem << " ";
			else
				os << "[] ";
		os << "};\t" << (item.used ? "" : "not ") << "used; " << (!item.definitions.empty() ? "" : "not ") << "defined";
		return os;
	}
}