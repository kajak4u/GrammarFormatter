#pragma once
#include "ObjectManager.h"
#include "MySet.h"
#include "main.h"

namespace GrammarSymbols
{

	class CTerminal;
	class CDefinition;

	//additional data assigned to symbol's name
	struct CDefinedSymbolManagerItem
	{
		//symbol's FIRST set
		MySet<CTerminal*, CompareObjects<CTerminal>> first;
		//symbol's FOLLOW set
		MySet<CTerminal*, CompareObjects<CTerminal>> follow;
		//symbol's definitions
		MySet<const CDefinition*> definitions;
		//whether symbol was used
		bool used = false;
		//number of instances with that name
		int instances = 0;
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CDefinedSymbolManagerItem& item);

	//manager class
	using CDefinedSymbolManager = CObjectManager<CDefinedSymbolManagerItem>;
}