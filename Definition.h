#pragma once

#include "Readable.h"
#include "GrammarObject.h"
#include "Spawnable.h"

namespace GrammarSymbols
{

	class IDefinition : public IReadable, public CGrammarObject, public ISpawnable
	{
	};

}