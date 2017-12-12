#pragma once

#include "Readable.h"
#include "Spawnable.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class CPrimary : public IReadable, public ISpawnable, public CGrammarObject
	{
	public:
		virtual bool Equals(const CPrimary* other) const = 0;
		virtual int Compare(const CPrimary* other) const { return this-other; };
		virtual bool operator<(const CPrimary& other) const { return Compare(&other) < 0; };
		CPrimary();
		virtual ~CPrimary();
	};

}