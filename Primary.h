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
		virtual int Compare(const CPrimary* other) const;
		virtual bool operator<(const CPrimary& other) const;
		CPrimary();
		virtual ~CPrimary();
	};

}