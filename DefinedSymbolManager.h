#pragma once
#include "ObjectManager.h"
#include "MySet.h"
#include "main.h"

namespace GrammarSymbols
{

	class CTerminal;
	class IDefinition;

	struct CDefinedSymbolManagerItem
	{
		MySet<CTerminal*, CompareObjects<CTerminal>> first;
		MySet<CTerminal*, CompareObjects<CTerminal>> follow;
		MySet<const IDefinition*> definitions;
		bool used = false;
	};
	_STD ostream& operator<<(_STD ostream& os, const CDefinedSymbolManagerItem& item);

	using CDefinedSymbolManager = CObjectManager<CDefinedSymbolManagerItem>;
}