#pragma once

#include "Readable.h"
#include "GrammarObject.h"
#include "Spawnable.h"

namespace GrammarSymbols
{
	//abstract base class for both CShortDefinition and CComplexDefinition
	class CDefinition : public IReadable, public CGrammarObject, public ISpawnable
	{
	};

}