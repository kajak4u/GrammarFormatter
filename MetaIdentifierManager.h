#pragma once
#include "ObjectManager.h"
#include "MySet.h"

class CTerminal;
class IDefinition;

struct CMetaIdentifierManagerItem
{
	MySet<CTerminal*> first;
	MySet<CTerminal*> follow;
	MySet<const IDefinition*> definitions;
	bool used = false;
};
std::ostream& operator<<(std::ostream& os, const CMetaIdentifierManagerItem& item);

using CMetaIdentifierManager = CObjectManager<CMetaIdentifierManagerItem>;