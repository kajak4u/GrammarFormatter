#pragma once

#include "Spawnable.h"
#include "Readable.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class CMultiplier : public ISpawnable, public IReadable, public CGrammarObject
	{
		int value;
	public:
		CMultiplier();
		virtual ~CMultiplier();

		_STD istream & ReadFrom(_STD istream & is) override;
		ISpawnable * spawn(bool copy = false) const override;
		int GetValue() const;
		static void registerPrefixes();

		void WriteTo(_STD ostream & os) const override;
	};
	_STD ostream& operator<<(_STD ostream& os, const CMultiplier& mult);
}