#pragma once
#include "ObjectManager.h"
#include "MySet.h"

class CTerminal;

struct CMetaIdentifierManagerItem
{
	MySet<CTerminal*> first;
	MySet<CTerminal*> follow;
	bool defined = false;
	bool used = false;
};
std::ostream& operator<<(std::ostream& os, const CMetaIdentifierManagerItem& item);

using CMetaIdentifierManager = CObjectManager<CMetaIdentifierManagerItem>;