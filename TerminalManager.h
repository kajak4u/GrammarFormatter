#pragma once
#include "ObjectManager.h"

class CTerminalManagerItem
{

};
std::ostream& operator<<(std::ostream& os, const CTerminalManagerItem&);

using CTerminalManager = CObjectManager<CTerminalManagerItem>;