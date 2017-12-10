#pragma once
#include "ObjectManager.h"
#include "MySet.h"

namespace GrammarSymbols
{

	class CTerminal;
	class IDefinition;

	struct CMetaIdentifierManagerItem
	{
		MySet<CTerminal*> first;
		MySet<CTerminal*> follow;
		MySet<const IDefinition*> definitions;
		bool used = false;
	};
	_STD ostream& operator<<(_STD ostream& os, const CMetaIdentifierManagerItem& item);

	using CMetaIdentifierManager = CObjectManager<CMetaIdentifierManagerItem>;
}